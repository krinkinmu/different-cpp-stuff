#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void print(boost::system::error_code const & /*err*/)
{
	std::cout << "Hello, World!\n";
}

int main()
{
	boost::asio::io_service io;
	boost::asio::deadline_timer t1(io, boost::posix_time::seconds(5));
	boost::asio::deadline_timer t2(io, boost::posix_time::seconds(1));
	t1.async_wait(&print);
	t2.async_wait(&print);
	io.run();
	return 0;
}
