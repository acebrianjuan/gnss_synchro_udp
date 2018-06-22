#ifndef GNSS_SYNCHRO_UDP_SINK_H_
#define GNSS_SYNCHRO_UDP_SINK_H_

#include <boost/asio.hpp>
#include "gnss_synchro.h"

class Gnss_Synchro_Udp_Sink
{
public:

	Gnss_Synchro_Udp_Sink(std::vector<std::string> addresses, const unsigned short &port);
	bool write_gnss_synchro(std::vector<Gnss_Synchro> stocks);

private:

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket socket;
	boost::system::error_code error;
	std::vector<boost::asio::ip::udp::endpoint> endpoints;
	std::vector<Gnss_Synchro> stocks;
};


#endif /* GNSS_SYNCHRO_UDP_SINK_H_ */
