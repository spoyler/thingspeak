#include <iostream>
#include <string>
#include <random>
#include <vector>

#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include "tcpcontext.h"
#include "thingspeak_channel.h"

struct BaseProgramParamester
{
    boost::optional<int> channel_id;
    boost::optional<std::string> read_key;
    boost::optional<std::string> write_key;
};

bool ParseCMDOptions(int argc, const char *argv[], BaseProgramParamester &params);

int main(int argc, const char *argv[])
{

    BaseProgramParamester params;
    if (!ParseCMDOptions(argc, argv, params))
        return 1;

    if (!params.channel_id || !params.read_key || !params.write_key)
        return 1;

//    const int channel_id = 337028;
//    const std::string read_key = "QCB8ZKKZEJBK7W6V";
//    const std::string write_key = "IMONY3UPBVR3AEO2";


    TCPContext tcp_context;
    tcp_context.Start();


    thing_speak::thingspeak_channel channel(*params.channel_id, *params.read_key,
                                            *params.write_key, tcp_context.IOService());

    thing_speak::ThingSpeakChannelStruct channel_info;
    std::vector<thing_speak::ThingSpeakChannelFeed> fields;

    thing_speak::ThingSpeakChannelFeed data;

    data.field[0] = 56;


    channel.UpdateChannelInfo(data);

    if (!channel.GetChennalData(1, channel_info, fields))
    {
        std::cout << channel_info.name << ": " << channel_info.description << std::endl;

        if(!fields.empty())
        {
            std::cout << fields[0].created_at << std::endl;            
            std::cout << fields[0].entry_id << std::endl;
            std::cout << fields[0].field[0] << std::endl;
        }
    }

    tcp_context.Stop();

	return 0;
}

bool ParseCMDOptions(int argc, const char *argv[], BaseProgramParamester &params)
{
    namespace po = boost::program_options;
    po::options_description cmd_description;
    cmd_description.add_options()
        ("help", "print all available options")
        ("id_channel", po::value<int>()->required(), "number of connecting chanel")
        ("read_key", po::value<std::string>()->required(), "You read key for the channel")
        ("write_key",po::value<std::string>()->required(), "You write key for the channel");

    po::variables_map cmd_options;
    po::store(po::parse_command_line(argc, argv, cmd_description), cmd_options);
    po::notify(cmd_options);

    if (cmd_options.count("help"))
    {
        std::cout << cmd_description << std::endl;
        return false;
    }

    for (const auto option : cmd_options)
    {
        if (option.first == "id_channel")
        {
            params.channel_id = option.second.as<int>();
            continue;
        }
        if (option.first == "read_key")
        {
            params.read_key = option.second.as<std::string>();
            continue;
        }
        if (option.first == "write_key")
        {
            params.write_key = option.second.as<std::string>();
            continue;
        }
    }

    return true;
}
