#include "iostream"
#include <boost/asio.hpp>

int main()
{
	boost::asio::io_service io_service;
	for (int i = 0; i < 42; ++i)
	{
		io_service.poll();
		std::cout << "Hello world!" <<std::endl;
	}
	return 0;
}
