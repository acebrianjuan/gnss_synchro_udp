#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>

#include "gnss_synchro.h"

// This example prints a serialized Gnss_Synchro object on the terminal.
int main(int argc, char* argv[])
{
  // Sample Gnss_Synchro object.
  Gnss_Synchro gnss_synchro = Gnss_Synchro();
  gnss_synchro.Channel_ID = 0;
  gnss_synchro.System = 'G';

  // Vector container to store Gnss_Synchro objects.
  std::vector<Gnss_Synchro> stocks;

  // Push Gnss_Synchro object into vector container.
  stocks.push_back(gnss_synchro);

  //
  boost::asio::streambuf b;

  // Stream.
  std::ostream os(&b); // <-- ostream instead of ostringstream !!!

  // Archive.
  boost::archive::text_oarchive archive(os);

  // Serialize data.
  archive << stocks;

  // Used to store information about errors that might arise.
  boost::system::error_code error;

  // Create the endpoint.
  boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1", error), 1337);

  //
  boost::asio::io_service io_service;

  // Instantiating an active UDP socket object.
  boost::asio::ip::udp::socket socket(io_service);

  // Open the socket.
  socket.open(endpoint.protocol(), error);

  // It is possible to ask the kernel to select a specific source IP and port
  // by calling bind() before calling connect() (optional).

  // Connect the socket to the endpoint.
  socket.connect(endpoint, error);

  // Try sending some data in input sequence.
  size_t n = socket.send(b.data());

  // Sent data is removed from input sequence.
  b.consume(n); 

  std::cout << "Sent" << std::endl;

  return 0;
}
