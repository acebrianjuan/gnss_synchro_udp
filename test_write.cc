#include "gnss_synchro_udp_sink.h"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc != 3)
    {
      std::cerr << "Usage: gnss_synchro_udp_sink <address> <port>" << std::endl;
      return 1;
    }

    unsigned short port = boost::lexical_cast<unsigned short>(argv[2]);
    Gnss_Synchro gnss_synchro;

	gnss_synchro.System='G';       //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
	gnss_synchro.Signal[0]='G';    //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
	gnss_synchro.Signal[1]='A';    //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
	gnss_synchro.Signal[2]='\0';   //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
	gnss_synchro.PRN=10;           //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
	gnss_synchro.Channel_ID=12;    //!< Set by Channel constructor
	// Acquisition
	gnss_synchro.Acq_delay_samples=0;           //!< Set by Acquisition processing block
	gnss_synchro.Acq_doppler_hz=0;              //!< Set by Acquisition processing block
	gnss_synchro.Acq_samplestamp_samples=0;     //!< Set by Acquisition processing block
	gnss_synchro.Flag_valid_acquisition=0;      //!< Set by Acquisition processing block
	//Tracking
	gnss_synchro.fs=0;                          //!< Set by Tracking processing block
	gnss_synchro. Prompt_I=0;                   //!< Set by Tracking processing block
	gnss_synchro. Prompt_Q=0;                   //!< Set by Tracking processing block
	gnss_synchro. CN0_dB_hz=0;                  //!< Set by Tracking processing block
	gnss_synchro. Carrier_Doppler_hz=0;         //!< Set by Tracking processing block
	gnss_synchro. Carrier_phase_rads=0;         //!< Set by Tracking processing block
	gnss_synchro. Code_phase_samples=0;         //!< Set by Tracking processing block
	gnss_synchro.Tracking_sample_counter=0;     //!< Set by Tracking processing block

	gnss_synchro. Flag_valid_symbol_output=0;   //!< Set by Tracking processing block
	gnss_synchro. correlation_length_ms=0;      //!< Set by Tracking processing block

	//Telemetry Decoder
	gnss_synchro. Flag_valid_word=0;            //!< Set by Telemetry Decoder processing block
	gnss_synchro. TOW_at_current_symbol_s=0;    //!< Set by Telemetry Decoder processing block

	// Observables
	gnss_synchro. Pseudorange_m=0;           //!< Set by Observables processing block
	gnss_synchro. RX_time=0;                 //!< Set by Observables processing block
	gnss_synchro. Flag_valid_pseudorange=0;  //!< Set by Observables processing block


	std::vector<Gnss_Synchro> stocks;

    Gnss_Synchro_Udp_Sink udp_sink(argv[1], port);

    int id = 0;
    for(int i = 1; i <= 1000; i++)
    {
	  gnss_synchro.Channel_ID = id;
	  stocks.push_back(gnss_synchro);
      id++;

      if(i % 4 == 0)
      {
    	  udp_sink.write_gnss_synchro(stocks);

    	  stocks.clear();

    	  id = 0;

    	  std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
