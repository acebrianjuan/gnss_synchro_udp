#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>

#include "gnss_synchro.h"

// This example prints a serialized Gnss_Synchro object on the terminal.
int main(int argc, char* argv[])
{
  // Used to store information about errors that might arise.
  boost::system::error_code error;

  // Create the endpoint.
  boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::v4(), 1337);

  //
  boost::asio::io_service io_service;

  // Instantiating an active UDP socket object.
  boost::asio::ip::udp::socket socket(io_service);

  // Open the socket.
  socket.open(endpoint.protocol(), error);

  // Bind the socket to the endpoint.
  socket.bind(endpoint, error);

  // Vector container to store Gnss_Synchro objects.
  std::vector<Gnss_Synchro> stocks;

  //
  boost::asio::streambuf b;

  // reserve 512 bytes in output sequence
  boost::asio::streambuf::mutable_buffers_type bufs = b.prepare(512);

  size_t n = socket.receive(bufs);

  // received data is "committed" from output sequence to input sequence
  b.commit(n);

  // Stream.
  std::istream is(&b);

  // Archive.
  boost::archive::text_iarchive archive(is);

  // Serialize data.
  archive >> stocks;

  //
  for (std::size_t i = 0; i < stocks.size(); ++i) {
    std::cout << "Gnss_Synchro packet number " << i << "\n";
    std::cout << "Channel_ID= "<<stocks[i].Channel_ID << std::endl;
    std::cout << "System= "<<stocks[i].System <<std::endl;
  }

  std::cout << "Received" << std::endl;

  return 0;
}
