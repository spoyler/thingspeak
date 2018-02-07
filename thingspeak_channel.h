#ifndef THINGSPEAK_CHANNEL_H
#define THINGSPEAK_CHANNEL_H

#include <string>

#include "http_client.h"

namespace thing_speak {

struct ThingSpeakChannelStruct
{
  int channel_id;
  float field[8];
  std::string created_at;
  uint entry_id;
  float status;
  float latitude;
  float longitude;
  float elevation;
};

class thingspeak_channel
{
    static const int kMaxChannelSize = 8;
    const std::string kHostName = "api.thingspeak.com";
public:
    thingspeak_channel(int channel_id, std::string read_key, std::string write_key);

    int UpdateChannelInfo(const ThingSpeakChannelStruct &last_data);
    int GetChennalData(std::vector<ThingSpeakChannelStruct> &last_data, int data_count);

private:

    std::stringstream& AddDataToUpdateRequest(const std::string &data_to_append,
                           const std::string &data_name,
                           std::stringstream &data);
private:
    const int m_channel_id;
    const std::string m_read_key;
    const std::string m_write_key;
    float m_field[kMaxChannelSize];
    http_client m_http_client;
};

}

#endif // THINGSPEAK_CHANNEL_H
