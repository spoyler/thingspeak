#include "thingspeak_channel.h"

#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace thing_speak {

enum CHANNEL_INFO {
    CHANNEL_ID,
    CHANNEL_NAME,
    CHANNEL_DESCRIPTION,
    CHANNEL_LATITUDE,
    CHANNEL_LONGITUDE,
    CHANNEL_LAST_ENTRY_ID
};


std::string random_sting()
{
    std::string base_sting("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(base_sting.begin(), base_sting.end(), generator);

    return base_sting.substr(0, 32);
}


thingspeak_channel::thingspeak_channel(int channel_id, std::string read_key, std::string write_key) :
    m_channel_id(channel_id),
    m_read_key(std::move(read_key)),
    m_write_key(std::move(write_key)),
    m_http_client(kHostName)
{
    m_channel_map["id"] = CHANNEL_ID;
    m_channel_map["name"] = CHANNEL_NAME;
    m_channel_map["description"] = CHANNEL_DESCRIPTION;
    m_channel_map["latitude"] = CHANNEL_LATITUDE;
    m_channel_map["longitude"] = CHANNEL_LONGITUDE;
    m_channel_map["last_entry_id"] = CHANNEL_LAST_ENTRY_ID;
}

int thingspeak_channel::UpdateChannelInfo(const ThingSpeakChannelFeed &data)
{
    std::stringstream head;
    head << "POST /update.json?";
    AddDataToUpdateRequest(m_write_key, "api_key", head);

    // add channel fields to request
    for (int i = 0; i < kMaxChannelSize; ++i)
    {
        head << "&";
        AddDataToUpdateRequest(std::to_string(data.field[i]), "field" + std::to_string(i + 1), head);
    }
    head << " HTTP/1.1\n";
    head << "Host:" << kHostName << "\n";
    head << "Cache-Control: no-cache\n";
    head << "Connection: keep-alive\n";
    head << "Content-Length: 0 \n\n";

    std::cout << head.str();

    std::vector<char> answer;
    m_http_client.SendMessage(head, answer);

    std::string str_output(answer.begin(), answer.end());

    std::cout << str_output << std::endl;

    return 0;
}

int thingspeak_channel::GetChennalData(int data_count,
        ThingSpeakChannelStruct &channel_info,
        std::vector<ThingSpeakChannelFeed> &fields)
{
    // create http_header
    std::stringstream head;
    head << "GET /channels/" << m_channel_id << "/feeds.json?";
    AddDataToUpdateRequest(m_read_key, "api_key", head) << "&";
    AddDataToUpdateRequest(std::to_string(data_count), "results", head) << " HTTP/1.1\n";
    head << "Host:" << kHostName << "\n";
    head << "Connection: keap-alive\n";
    head << "Content-Length: 0 \n\n";

    // send request
    std::vector<char> answer;
    m_http_client.SendMessage(head, answer);

    ParseAnswer(answer, channel_info, fields);

    return 0;
}

std::stringstream &thingspeak_channel::AddDataToUpdateRequest(const std::string &data_to_append,
                                                const std::string &data_name,
                                                std::stringstream &data)
{

    data << data_name << "=" << data_to_append;
    return data;
}

void thingspeak_channel::ParseAnswer(const std::vector<char> &answer,
                 ThingSpeakChannelStruct &channel_info,
                 std::vector<ThingSpeakChannelFeed> &last_data)
{
    std::string string_answer(answer.begin(), answer.end());

    const int header_size = string_answer.find("\r\n\r\n");

    if (header_size)
        string_answer = string_answer.substr(header_size + sizeof("\r\n\r\n") - 1);
    else
        return;

    string_answer.erase(string_answer.find_last_of("}") + 1);
    string_answer = string_answer.substr(string_answer.find('{'));

    std::stringstream ss(string_answer);

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);

    for (const auto &array_element : pt)
    {
        // find channel info
        if (array_element.first == "channel")
        {
            for (const auto &element : array_element.second)
            {
                const auto it = m_channel_map.find(element.first);
                if (it == m_channel_map.end())
                {
                    //std::cout << element.first.data() << ": " << element.second.data() << std::endl;
                    continue;
                }

                switch (it->second)
                {
                    case CHANNEL_ID:
                        channel_info.channel_id = std::stoi(element.second.data());
                        break;
                    case CHANNEL_NAME:
                        channel_info.name = element.second.data();
                        break;
                    case CHANNEL_DESCRIPTION:
                        channel_info.description = element.second.data();
                        break;
                    case CHANNEL_LATITUDE:
                        channel_info.latitude = std::stof(element.second.data());
                        break;
                    case CHANNEL_LONGITUDE:
                        channel_info.longitude = std::stof(element.second.data());
                        break;
                    case CHANNEL_LAST_ENTRY_ID:
                        channel_info.last_entry_id = std::stoi(element.second.data());
                        break;
                }
            }
        }
        // parse data array
        if (array_element.first == "feeds")
        {
            // walk through all feeds
            for (const auto &feeds : array_element.second)
            {
                ThingSpeakChannelFeed data;
                for(const auto &feed : feeds.second)
                {
                    if (feed.first == "entry_id")
                    {
                        data.entry_id = std::stol(feed.second.data());
                    }
                    if (feed.first == "created_at")
                    {
                        data.created_at = feed.second.data();
                    }
                    if (feed.first.find("field") != std::string::npos)
                    {
                        uint field_id = std::stol(feed.first.substr(sizeof("field") - 1));
                        if (field_id < 8)
                        {
                            data.field[field_id - 1] = std::stof(feed.second.data());
                        }
                        std::cout << feed.first.data() << ": " << feed.second.data() << std::endl;
                    }
                }

                last_data.push_back(std::move(data));
            }
        }
    }

}

}

























