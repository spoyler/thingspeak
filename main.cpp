#include <iostream>
#include <string>
#include <random>

#include <boost/asio.hpp>

std::string random_sting()
{
    std::string base_sting("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    
    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(base_sting.begin(), base_sting.end(), generator);

    return base_sting.substr(0, 32);    
}

int main()
{
	using namespace boost::asio;
	io_service io_service;
	ip::tcp::resolver resolver(io_service);
	ip::tcp::resolver::query query("api.thingspeak.com", "http");

	auto endpoint_iterator = resolver.resolve(query);
	decltype(endpoint_iterator) end;
	
	ip::tcp::socket socket(io_service);
        const std::string key = "QCB8ZKKZEJBK7W6V";
        const std::string write_key = "IMONY3UPBVR3AEO2";
        std::string head;
        

	while(endpoint_iterator != end)
	{

		ip::tcp::socket socket(io_service);
		socket.open(ip::tcp::v4());
                
                ip::tcp::endpoint endpoint = *endpoint_iterator;
		std::cout << endpoint << std::endl;

		boost::system::error_code error;
                ip::tcp::endpoint static_endpoint(ip::address::from_string("52.1.229.129"), 80);
 
		socket.connect(*endpoint_iterator, error);

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

		char buff1[2048];
		socket.read_some(buffer(buff1,2048), error);
                
                if (!error)
                {
                    std::cout << "read ok" << std::endl;
                    socket.shutdown(ip::tcp::socket::shutdown_receive);
                    socket.close();

                    std::cout << buff1 << std::endl;
                
                }
                else
                {
                    std::cout << error.message() << std::endl;
                }
               
  
  
                std::string boundary ("----");
                boundary += random_sting();
                boundary += "\n";
                boundary += "\n";
                
/*                
                std::string data;                
                
                data += boundary;                   
                data += "Content-Disposition: form-data; name=\"api_key\"";
                data += "\r\n";
                data += "\r\n";
                data += write_key; // this is data
                data += "\r\n";
                data += boundary;
                  
                data += "Content-Disposition: form-data; name=\"field1\"";
                data += "\r\n";
                data += "\r\n";
                data += std::to_string(55); // this is data
                data += "\r\n";
                data += boundary;
                data += "\r\n";
                data += "\r\n";
*/
                
                    
                head.clear();
                head += "POST /update.json?api_key=IMONY3UPBVR3AEO2&amp;field1=99 HTTP/1.1\n";
                head += "Host: api.thingspeak.com\n";
                head += "Cache-Control: no-cache\n\n";
                //head += "Connection: close\r\n";
                //head += "Keep-Alive: 300\r\n";
                //head += "Postman-Token: 86ef0833-bf2f-952c-d65d-2d4c0920a08b\n";
                //head += "Content-Type: multipart/form-data;  boundary="; head += boundary;
                
                //head += "Content-Length:"; head += std::to_string(0); head += "\r\n\r\n";
                     
                //head += data;
                        
                std::cout << head << std::endl;
                        
                                             
                        
                //socket.connect(*endpoint_iterator, error);
                socket.connect(static_endpoint, error);
                
                
                
                if(error)
                {
                   std::cout << error.message() << std::endl; 
                }
                
                socket.write_some(buffer(head.c_str(), head.size()), error);
                
                if(error)
                {
                   std::cout << error.message() << std::endl; 
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
  
            break;
                

	}


	return 0;
}
