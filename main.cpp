#include "iostream"

#include <boost/asio.hpp>

int main()
{
	using namespace boost::asio;
	io_service io_service;
	ip::tcp::resolver resolver(io_service);
	ip::tcp::resolver::query query("api.thingspeak.com", "https");

	auto endpoint_iterator = resolver.resolve(query);
	decltype(endpoint_iterator) end;
	
	ip::tcp::socket socket(io_service);


	while(endpoint_iterator != end)
	{

		ip::tcp::socket socket(io_service);
		socket.open(ip::tcp::v4());

		boost::system::error_code error;
		socket.connect(*endpoint_iterator, error);

		if (!error)
		{
			std::cerr << "connected" << std::endl;
		}
		else
		{
			std::cerr << error.message() << std::endl;
			break;
		}


		socket.write_some(buffer("GET /channels/337028/feeds.json?api_key=QCB8ZKKZEJBK7W6V&results=2\r\n"),
				error);

		if (!error)
		{
			std::cerr << "send sucsess" << std::endl;
		}
		else
		{
			std::cerr << error.message() << std::endl;
			break;
		}

		char buff[1024];
		socket.read_some(buffer(buff,1024));
		socket.shutdown(ip::tcp::socket::shutdown_receive);
		socket.close();

		std::cout << buff << std::endl;

		//ip::tcp::endpoint endpoint = *endpoint_iterator++;
		//std::cout << endpoint << std::endl;
	}


	return 0;
}
