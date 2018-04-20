#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include <thread>

#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include "tcpcontext.h"
#include "thingspeak_channel.h"

#include "pi_dht_read.h"

struct BaseProgramParamester
{
    boost::optional<int> channel_id;
    boost::optional<std::string> read_key;
    boost::optional<std::string> write_key;
};

static const char *kBaseConfigFilename = "config.cfg";

bool ParseCMDOptions(int argc, const char *argv[], BaseProgramParamester &params);


std::deque<thing_speak::ThingSpeakChannelFeed> sensor_queue;
std::mutex lock_queue;
std::condition_variable wait_queue;

void ReadDataFromSensor(std::chrono::seconds sleep_time)
{
	while(1)
	{
		thing_speak::ThingSpeakChannelFeed data;

		while(1)
		{

			if (pi_dht_read(DHT11, 4, &data.field[1], &data.field[0]) == DHT_SUCCESS)
			{
				std::cout << "temperature" << data.field[0] << std::endl;
				std::cout << "humidity " << data.field[1] << std::endl;
                {
                    std::lock_guard<std::mutex> lock(lock_queue);
                    sensor_queue.push_back(std::move(data));
                    wait_queue.notify_all();
                }
				std::this_thread::sleep_for(sleep_time);              
				break;
			}
		}
	}
}


int main(int argc, const char *argv[])
{

    BaseProgramParamester params;
    if (!ParseCMDOptions(argc, argv, params))
        return 1;

    if (!params.channel_id || !params.read_key || !params.write_key)
    {
        return 1;
    }

    TCPContext tcp_context;
    tcp_context.Start();


    thing_speak::thingspeak_channel channel(*params.channel_id, *params.read_key,
                                            *params.write_key, tcp_context.IOService());

    std::thread read_data_from_sensor(ReadDataFromSensor, std::chrono::seconds(60));

    while (1)
    {
        std::unique_lock<std::mutex> lock(lock_queue);
        wait_queue.wait(lock, [](){return !sensor_queue.empty();});

        if (!sensor_queue.empty())
        {
            auto data = sensor_queue.front();
            sensor_queue.pop_front();
            channel.UpdateChannelInfo(data);
        }
    }

    tcp_context.Stop();

	return 0;
}

bool ParseCMDOptions(int argc, const char *argv[], BaseProgramParamester &params)
{

    namespace po = boost::program_options;

    // params for cmd
    po::options_description cmd_description;
    cmd_description.add_options()
        ("help", "print all available options")
        ("cfg_file", po::value<std::string>()->default_value(kBaseConfigFilename), "put here you file name config");

    // params for config file
    po::options_description config_file_description;
    config_file_description.add_options()
        ("id_channel", po::value<int>()->required(), "number of connecting chanel")
        ("read_key", po::value<std::string>()->required(), "You read key for the channel")
        ("write_key",po::value<std::string>()->required(), "You write key for the channel");


    // read file name from cmd line
    po::variables_map cmd_params;
    try {
        po::store(po::parse_command_line(argc, argv, cmd_description), cmd_params);
        po::notify(cmd_params);
    }
    catch(std::exception const &ex)
    {
        std::cerr << "Error, invalid cmd params: "  << ex.what() << std::endl;
        return false;
    }
    
    // print help for users
    if (cmd_params.count("help"))
    {
        std::cout << cmd_description << std::endl;
        return false;
    }

    // read file name from params
    std::string file_name;
    if (cmd_params.count("cfg_file"))
    {
        file_name = cmd_params["cfg_file"].as<std::string>();
    }

    // read channel config form config file
    po::variables_map config_params;
    try {
        po::store(po::parse_config_file<char>(file_name.c_str(), config_file_description), config_params);
        po::notify(config_params);
    }
    catch(std::exception const &ex)
    {
        std::cerr << "Error reading confing file :" << ex.what() << std::endl;
        return false;
    }

    for (const auto param : config_params)
    {
        if (param.first == "id_channel")
        {
            params.channel_id = param.second.as<int>();
            continue;
        }
        if (param.first == "read_key")
        {
            params.read_key = param.second.as<std::string>();
            continue;
        }
        if (param.first == "write_key")
        {
            params.write_key = param.second.as<std::string>();
            continue;
        }
    }

    return true;
}
