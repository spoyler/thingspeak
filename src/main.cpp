#include <iostream>
#include <string>
#include <random>
#include <vector>

#include <boost/asio.hpp>

#include "tcpcontext.h"
#include "thingspeak_channel.h"


int main()
{
    const int channel_id = 337028;
    const std::string read_key = "QCB8ZKKZEJBK7W6V";
    const std::string write_key = "IMONY3UPBVR3AEO2";

    TCPContext tcp_context;
    tcp_context.Start();


    thing_speak::thingspeak_channel channel(channel_id, read_key,
                                            write_key, tcp_context.IOService());

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
