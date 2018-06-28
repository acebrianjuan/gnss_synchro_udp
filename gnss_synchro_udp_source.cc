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
