#ifndef THINGSPEAK_CHANNEL_H
#define THINGSPEAK_CHANNEL_H

#include <string>

#include "http_client.h"

namespace thing_speak {

class thingspeak_channel
{
    static const int MaxChannelSize = 8;
public:
    thingspeak_channel(std::string read_key, std::string write_key);

    int UpdateChannelInfo();
    int GetChannelInfo();

private:

    AddDataToUpdateRequest(const std::string &data_to_append, const std::string &data_name,
                           const std::string &boundary, std::stringstream &data);

    const std::string m_read_key;
    const std::string m_write_key;
    int m_channel_id;
    float m_field[MaxChannelSize];
    http_client m_http_client;
};

}

#endif // THINGSPEAK_CHANNEL_H
