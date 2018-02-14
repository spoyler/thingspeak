#ifndef THINGSPEAK_CHANNEL_H
#define THINGSPEAK_CHANNEL_H

#include <string>
#include <map>

#include "http_client.h"

namespace thing_speak {

struct ThingSpeakChannelStruct
{
  int channel_id;
  std::string name;
  std::string description;
  float latitude;
  float longitude;
  float elevation;
  int last_entry_id;
};

struct ThingSpeakChannelFeed
{
    uint entry_id;
    std::string created_at;
    float field[8];
};

class thingspeak_channel
{
    static const int kMaxChannelSize = 8;
    const std::string kHostName = "api.thingspeak.com";
public:
    thingspeak_channel(int channel_id, std::string read_key,
                       std::string write_key, boost::asio::io_service &io_service);

    int UpdateChannelInfo(const ThingSpeakChannelFeed &last_data);
    int GetChennalData(int data_count, ThingSpeakChannelStruct &channel_info,
                                        std::vector<ThingSpeakChannelFeed> &fields);

private:

    std::stringstream& AddDataToUpdateRequest(const std::string &data_to_append,
                                               const std::string &data_name,
                                               std::stringstream &data);
    void ParseAnswer(const std::vector<char> &answer,
                     ThingSpeakChannelStruct &channel_info,
                     std::vector<ThingSpeakChannelFeed> &last_data);
private:
    const int m_channel_id;
    const std::string m_read_key;
    const std::string m_write_key;
    float m_field[kMaxChannelSize];
    http_client m_http_client;
    std::map<std::string, int> m_channel_map;
};

}

#endif // THINGSPEAK_CHANNEL_H
