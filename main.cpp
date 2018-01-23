#include "iostream"

#include <boost/asio.hpp>

int main()
{
	using namespace boost::asio;
	io_service io_service;
	ip::tcp::resolver resolver(io_service);
	ip::tcp::resolver::query query("api.thingspeak.com", "http");

	auto endpoint_iterator = resolver.resolve(query);
	decltype(endpoint_iterator) end;
	
	ip::tcp::socket socket(io_service);
        

	while(endpoint_iterator != end)
	{

		ip::tcp::socket socket(io_service);
		socket.open(ip::tcp::v4());
                
                ip::tcp::endpoint endpoint = *endpoint_iterator;
		std::cout << endpoint << std::endl;

		boost::system::error_code error;
		socket.connect(*endpoint_iterator++, error);

		if (!error)
		{
			std::cout << "connected" << std::endl;
		}
		else
		{
			std::cerr << error.message() << std::endl;
			break;
		}
                
                std::string head;
                const std::string key = "QCB8ZKKZEJBK7W6V";
                
                
                 head += "GET /channels/337028/feeds.json HTTP/1.1\r\n";
                 head += "Host: api.thingspeak.com\r\n";
                        head += "Connection: close\r\n";
                        head += "X-THINGSPEAKAPIKEY: "; head += key; head += "\r\n";
                        head += "Content-Type: application/x-www-form-urlencoded\r\n";
                        head += "Content-Length: 0 \r\n\r\n";
                 
                 
                std::cout << head << std::endl; 


		socket.write_some(buffer(head.c_str(), head.size()), error);

		if (!error)
		{
			std::cerr << "send sucsess" << std::endl;
		}
		else
		{
			std::cerr << error.message() << std::endl;
			break;
		}

		char buff[2048];
		socket.read_some(buffer(buff,2048), error);
                
                if (!error)
                {
                    std::cout << "read ok" << std::endl;
                    socket.shutdown(ip::tcp::socket::shutdown_receive);
                    socket.close();

                    std::cout << buff << std::endl;
                
                    break;
                }
                else
                {
                    std::cout << error.message() << std::endl;
                }
                

	}


	return 0;
}
