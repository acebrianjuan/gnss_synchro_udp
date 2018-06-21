#include "gnss_synchro_udp_sink.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>
#include <iostream>

Gnss_Synchro_Udp_Sink::Gnss_Synchro_Udp_Sink(std::string address, const unsigned short &port) : socket{io_service},
endpoint{boost::asio::ip::address::from_string(address, error), port}
{
	socket.open(endpoint.protocol(), error);
	socket.connect(endpoint, error);
}

bool Gnss_Synchro_Udp_Sink::write_gnss_synchro(std::vector<Gnss_Synchro> stocks)
{
	std::ostringstream archive_stream;
	boost::archive::binary_oarchive oa{archive_stream};
	oa << stocks;
	std::string outbound_data = archive_stream.str();

	try
	{
		socket.send(boost::asio::buffer(outbound_data));
	}
	catch (boost::system::system_error const& e)
	{
		return false;
	}

	return true;
}
