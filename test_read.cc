/*!
 * \file test_read.cc
 * \brief Terminal-based testing utility that listens on the specified udp
 * port and recovers incomming serialized gnss_synchro objects
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
