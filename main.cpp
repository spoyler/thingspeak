#include "iostream"
#include <boost/asio.hpp>

int main()
{
	using namespace boost::asio;
	io_service io_service;
	ip::tcp::resolver resolver(io_service);
	ip::tcp::resolver::query query("api.thingspeak.com", "https");

	auto resolve = resolver.resolve(query);
	decltype(resolve) end;
	
	while(resolve != end)
	{

		ip::tcp::endpoint endpoint = *resolve++;
		std::cout << endpoint << std::endl;
	}


	return 0;
}

