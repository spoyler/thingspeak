#include "thingspeak_channel.h"

#include <sstream>

namespace thing_speak {

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
}

int thingspeak_channel::UpdateChannelInfo(const ThingSpeakChannelStruct &data)
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

int thingspeak_channel::GetChennalData(std::vector<ThingSpeakChannelStruct> &last_data,
                                       int data_count)
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

    return 0;
}

std::stringstream &thingspeak_channel::AddDataToUpdateRequest(const std::string &data_to_append,
                                                const std::string &data_name,
                                                std::stringstream &data)
{

    data << data_name << "=" << data_to_append;
    return data;
}

}
