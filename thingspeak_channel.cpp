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


thingspeak_channel::thingspeak_channel(std::string read_key, std::string write_key) :
    m_read_key(std::move(read_key)),
    m_write_key(std::move(write_key))
{


}

int thingspeak_channel::UpdateChannelInfo()
{
    std::stringstream head;

    std::string boundary = random_sting();

    std::stringstream message_content;

    AddDataToUpdateRequest(m_write_key, "api_key", boundary, message_content);
    AddDataToUpdateRequest(std::to_string(100), field1, boundary, message_content);


    head << "POST /update.json?api_key=IMONY3UPBVR3AEO2&amp;field1=99 HTTP/1.1\n";
    head << "Host: api.thingspeak.com\n";
    head << "Cache-Control: no-cache\n";
    head << "Connection: keep-alive\n";
    head << "Keep-Alive: 300\n";
    //head += "Postman-Token: 86ef0833-bf2f-952c-d65d-2d4c0920a08b\n";
    head << "Content-Type: multipart/form-data;  boundary=" << boundary << "\n";

    head << "Content-Length:" << message_content.str().size() << "\n\n";

    // compile full message
    head << message_content;

    m_http_client.SendMessage(head);

}

int thingspeak_channel::GetChannelInfo()
{
    std::stringstream head;
    head << "GET /channels/337028/feeds.json HTTP/1.1\n";
    head << "Host: api.thingspeak.com\n";
    head << "Connection: close\n";
    head << "X-THINGSPEAKAPIKEY: " << m_read_key << "\n";
    head << "Content-Type: application/x-www-form-urlencoded\n";
    head << "Content-Length: 0 \n\n";
    m_http_client.SendMessage(head);
}

void thingspeak_channel::AddDataToUpdateRequest(const std::string &boundary, std::stringstream &data)
{
}

}
