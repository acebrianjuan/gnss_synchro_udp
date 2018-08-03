/*!
 * \file test_write.cc
 * \brief Terminal-based testing utility that sends serialized gnss_synchro
 * objects to the specified destination addresses and port over udp
 * \author Álvaro Cebrián Juan, 2018. acebrianjuan(at)gmail.com
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2018  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <https://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */

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
            if (argc < 3)
                {
                    std::cerr << "Usage: gnss_synchro_udp_sink <address> <port>" << std::endl;
                    return false;
                }

            std::vector<Gnss_Synchro> stocks;
            Gnss_Synchro gnss_synchro;

            gnss_synchro.System = 'G';      //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
            gnss_synchro.Signal[0] = 'G';   //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
            gnss_synchro.Signal[1] = 'A';   //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
            gnss_synchro.Signal[2] = '\0';  //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
            gnss_synchro.PRN = 10;          //!< Set by Channel::set_signal(Gnss_Signal gnss_signal)
            gnss_synchro.Channel_ID = 12;   //!< Set by Channel constructor
            // Acquisition
            gnss_synchro.Acq_delay_samples = 0;        //!< Set by Acquisition processing block
            gnss_synchro.Acq_doppler_hz = 0;           //!< Set by Acquisition processing block
            gnss_synchro.Acq_samplestamp_samples = 0;  //!< Set by Acquisition processing block
            gnss_synchro.Flag_valid_acquisition = 0;   //!< Set by Acquisition processing block
            //Tracking
            gnss_synchro.fs = 2000000;                 //!< Set by Tracking processing block
            gnss_synchro.Prompt_I = 0;                 //!< Set by Tracking processing block
            gnss_synchro.Prompt_Q = 0;                 //!< Set by Tracking processing block
            gnss_synchro.CN0_dB_hz = 0;                //!< Set by Tracking processing block
            gnss_synchro.Carrier_Doppler_hz = 0;       //!< Set by Tracking processing block
            gnss_synchro.Carrier_phase_rads = 0;       //!< Set by Tracking processing block
            gnss_synchro.Code_phase_samples = 0;       //!< Set by Tracking processing block
            gnss_synchro.Tracking_sample_counter = 0;  //!< Set by Tracking processing block

            gnss_synchro.Flag_valid_symbol_output = 0;  //!< Set by Tracking processing block
            gnss_synchro.correlation_length_ms = 0;     //!< Set by Tracking processing block

            //Telemetry Decoder
            gnss_synchro.Flag_valid_word = 0;           //!< Set by Telemetry Decoder processing block
            gnss_synchro.TOW_at_current_symbol_ms = 0;  //!< Set by Telemetry Decoder processing block

            // Observables
            gnss_synchro.Pseudorange_m = 0;           //!< Set by Observables processing block
            gnss_synchro.RX_time = 0;                 //!< Set by Observables processing block
            gnss_synchro.Flag_valid_pseudorange = 0;  //!< Set by Observables processing block
            gnss_synchro.interp_TOW_ms = 0;           //!< Set by Observables processing block


            unsigned short port = boost::lexical_cast<unsigned short>(argv[argc - 1]);
            std::vector<std::string> addresses;

            for (int i = 1; i < argc - 1; i++)
                {
                    addresses.push_back(argv[i]);
                }

            Gnss_Synchro_Udp_Sink udp_sink(addresses, port);

            // Test 1:
            for (int i = 1; i <= 20000; i++)
                {
                    for (int id = 0; id < 8; id++)
                        {
                            gnss_synchro.Channel_ID = id;
                            gnss_synchro.CN0_dB_hz = i;

                            stocks.push_back(gnss_synchro);
                            udp_sink.write_gnss_synchro(stocks);
                            stocks.clear();
                        }
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
        }
    catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }

    return true;
}
