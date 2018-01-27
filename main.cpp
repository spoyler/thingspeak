#include <iostream>
#include <string>
#include <random>
#include <vector>

#include <boost/asio.hpp>

#include "thingspeak_channel.h"



int main()
{
    const int channel_id = 337028;
    const std::string read_key = "QCB8ZKKZEJBK7W6V";
    const std::string write_key = "IMONY3UPBVR3AEO2";

    std::cout << read_key << std::endl;

    thing_speak::thingspeak_channel channel(channel_id, read_key, write_key);

    std::vector<thing_speak::ThingSpeakChannelStruct> channel_data;

    thing_speak::ThingSpeakChannelStruct data;

    data.field[0] = 15;


    channel.UpdateChannelInfo(data);


    if (!channel.GetChennalData(channel_data, 1))
    {
        if(!channel_data.empty())
        {
            std::cout << channel_data[0].created_at << std::endl;
            std::cout << channel_data[0].entry_id << std::endl;
        }
    }



	return 0;
}
