/*!
 * \file gnss_synchro_udp_source.cc
 * \brief Implementation of a class that recovers serialized gnss_synchro
 * objects from a udp socket
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

#include "gnss_synchro_udp_source.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <sstream>
#include <iostream>

Gnss_Synchro_Udp_Source::Gnss_Synchro_Udp_Source(const unsigned short& port) : socket{io_service},
                                                                               endpoint{boost::asio::ip::udp::v4(), port}
{
    socket.open(endpoint.protocol(), error);
    socket.bind(endpoint, error);
    pnum = 0;
}

bool Gnss_Synchro_Udp_Source::read_gnss_synchro(std::vector<Gnss_Synchro>& stocks)
{
    char buff[1500];
    int bytes = socket.receive(boost::asio::buffer(buff));

    try
        {
            std::string archive_data(&buff[0], bytes);
            std::istringstream archive_stream(archive_data);
            boost::archive::binary_iarchive archive(archive_stream);
            archive >> stocks;
        }
    catch (std::exception& e)
        {
            return false;
        }

    return true;
}

bool Gnss_Synchro_Udp_Source::print_gnss_synchro()
{
    if (read_gnss_synchro(stocks))
        {
            pnum++;
            std::cout << "Gnss_Synchro packet number " << pnum << std::endl;

            for (std::size_t i = 0; i < stocks.size(); i++)
                {
                    // Satellite and signal information
                    std::cout << "│" << std::endl;
                    std::cout << "├── Satellite and signal information" << std::endl;
                    std::cout << "│   ├── System = " << stocks[i].System << std::endl;
                    std::cout << "│   ├── Signal = " << stocks[i].Signal << std::endl;
                    std::cout << "│   ├── PRN = " << stocks[i].PRN << std::endl;
                    std::cout << "│   └── Channel_ID = " << stocks[i].Channel_ID << std::endl;
                    std::cout << "│" << std::endl;
                    // Acquisition
                    std::cout << "├── Acquisition" << std::endl;
                    std::cout << "│   ├── Acq_delay_samples = " << stocks[i].Acq_delay_samples << std::endl;
                    std::cout << "│   ├── Acq_doppler_hz = " << stocks[i].Acq_doppler_hz << std::endl;
                    std::cout << "│   ├── Acq_samplestamp_samples = " << stocks[i].Acq_samplestamp_samples << std::endl;
                    std::cout << "│   ├── Acq_doppler_step = " << stocks[i].Acq_doppler_step << std::endl;
                    std::cout << "│   └── Flag_valid_acquisition = " << stocks[i].Flag_valid_acquisition << std::endl;
                    std::cout << "│" << std::endl;
                    // Tracking
                    std::cout << "├── Tracking" << std::endl;
                    std::cout << "│   ├── fs = " << stocks[i].fs << std::endl;
                    std::cout << "│   ├── Prompt_I = " << stocks[i].Prompt_I << std::endl;
                    std::cout << "│   ├── Prompt_Q = " << stocks[i].Prompt_Q << std::endl;
                    std::cout << "│   ├── CN0_dB_hz = " << stocks[i].CN0_dB_hz << std::endl;
                    std::cout << "│   ├── Carrier_Doppler_hz = " << stocks[i].Carrier_Doppler_hz << std::endl;
                    std::cout << "│   ├── Carrier_phase_rads = " << stocks[i].Carrier_phase_rads << std::endl;
                    std::cout << "│   ├── Code_phase_samples = " << stocks[i].Code_phase_samples << std::endl;
                    std::cout << "│   ├── Tracking_sample_counter = " << stocks[i].Tracking_sample_counter << std::endl;
                    std::cout << "│   ├── Flag_valid_symbol_output = " << stocks[i].Flag_valid_symbol_output << std::endl;
                    std::cout << "│   └── correlation_length_ms = " << stocks[i].correlation_length_ms << std::endl;
                    std::cout << "│" << std::endl;
                    // Telemetry Decoder
                    std::cout << "├── Telemetry Decoder" << std::endl;
                    std::cout << "│   ├── Flag_valid_word = " << stocks[i].Flag_valid_word << std::endl;
                    std::cout << "│   └── TOW_at_current_symbol_ms = " << stocks[i].TOW_at_current_symbol_ms << std::endl;
                    std::cout << "│" << std::endl;
                    // Observables
                    std::cout << "└── Observables" << std::endl;
                    std::cout << "    ├── Pseudorange_m = " << stocks[i].Pseudorange_m << std::endl;
                    std::cout << "    ├── RX_time = " << stocks[i].RX_time << std::endl;
                    std::cout << "    ├── Flag_valid_pseudorange = " << stocks[i].Flag_valid_pseudorange << std::endl;
                    std::cout << "    ├── interp_TOW_ms = " << stocks[i].interp_TOW_ms << std::endl;
                    std::cout << "    └── Channel_ID = " << stocks[i].Channel_ID << std::endl;
                    std::cout << std::endl;
                }
        }
    else
        {
            return false;
        }

    return true;
}
