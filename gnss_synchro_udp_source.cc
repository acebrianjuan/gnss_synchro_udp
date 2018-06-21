#include "gnss_synchro_udp_source.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>
#include <iostream>

Gnss_Synchro_Udp_Source::Gnss_Synchro_Udp_Source(const unsigned short &port) : socket{io_service},
endpoint{boost::asio::ip::udp::v4(), port}
{
	socket.open(endpoint.protocol(), error);
	socket.bind(endpoint, error);
}

//bool Gnss_Synchro_Udp_Source::read_gnss_synchro(const boost::system::error_code& e)
bool Gnss_Synchro_Udp_Source::read_gnss_synchro()
{
	char buff[1500];
	int bytes = socket.receive(boost::asio::buffer(buff));

    try
    {
    	std::string archive_data(&buff[0], bytes);
    	std::istringstream archive_stream(archive_data);
    	boost::archive::binary_iarchive archive(archive_stream);
    	archive >> stocks;

    	for (std::size_t i = 0; i < stocks.size(); i++)
    	{
    		std::cout << "Channel_ID= " << stocks[i].Channel_ID << std::endl;
    	}
    }
    catch (std::exception& e)
    {
    	return false;
    }

	return true;
}
