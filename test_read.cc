#include "gnss_synchro_udp_source.h"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc != 2)
    {
    	std::cerr << "Usage: gnss_synchro_udp_source <port>" << std::endl;
    	return false;
    }

    unsigned short port = boost::lexical_cast<unsigned short>(argv[1]);
    Gnss_Synchro_Udp_Source udp_source(port);

    int i = 1;
    while (true)
    {
    	udp_source.read_gnss_synchro();

    	std::cout << "Gnss_Synchro packet number " << i << std::endl;
    	i++;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return true;
}
