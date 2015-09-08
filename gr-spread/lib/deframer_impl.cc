/* -*- c++ -*- */
/* 
 * Copyright 2014 Paul David <pudavid@vt.edu>
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/block_detail.h>

#include "deframer_impl.h"
#include "format.h"

#include <iostream>
#include <boost/crc.hpp>
#include <stdexcept>

namespace gr {
  namespace Spread {

    deframer::sptr
    deframer::make(int test)
    {
      return gnuradio::get_initial_sptr
        (new deframer_impl(test));
    }

    /*
     * The private constructor
     */
    deframer_impl::deframer_impl(int test)
      : gr::block("deframer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        frame_data(NULL), 
        frame_len(0), 
        received(0), 
        passed(0)
    {
        message_port_register_in(pmt::mp("in"));

        set_msg_handler(pmt::mp("in"), boost::bind(&deframer_impl::pmt_in, this, _1)); 
    }

    void deframer_impl::pmt_in(pmt::pmt_t msg)
    {

        const char *pdu; 
        size_t      msg_len; 
    
        if (pmt::is_blob(msg)) {
           
            pdu = (const char *) blob_data(msg); 
            msg_len = blob_length(msg);  

        } else if (pmt::is_eof_object(msg)) {

            detail().get()->set_done(true); 
            return; 
             
        } else {

            throw std::invalid_argument("Deframer only accepts PDUs"); 
    
        }

        // Checksum passes?
        parse_frame(pdu, msg_len); 

        delete[] frame_data; 
        frame_data = NULL; 
             
    }

    void deframer_impl::parse_frame(const char *pdu_data, const int pdu_len) 
    {
        // Create the header
        frame_header* header = (frame_header *) pdu_data; 

        // Calculate the length of the data within the frame 
        // minus the fcs length
        size_t data_len = pdu_len - sizeof(frame_header) - 4; 

        boost::crc_32_type data; 
        data.process_bytes(pdu_data, sizeof(frame_header) + data_len);

        uint32_t  result = data.checksum();  
        uint32_t  fcs; 

        std::memcpy(&fcs, pdu_data + sizeof(frame_header) + data_len, 4);

        if (result == fcs) 
            passed++; 

        received++;

        std::cout << "--------------------------------------" << std::endl;
        std::cout << "INCOMING CRC: " << result << std::endl; 
        std::cout << "READ FCS:     " << fcs << std::endl; 

        print(header->type, header->len, result == fcs); 
    } 

    void deframer_impl::print(const uint8_t type, const uint8_t length, const bool result) 
    {
        std::cout << "--------------------------------------"
                  << std::endl; 
        std::cout << "FRAME TYPE:                       " 
                  << (int) type << std::endl; 
        std::cout << "FRAME LENGTH:                     "
                  << (int) length << std::endl; 
        std::cout << "CHECKSUM RESULT:                  " 
                  << (int) result << std::endl;          
        std::cout << "Detected frames:                  "
                  << received << std::endl;  
        std::cout << "Passed frames:                    "
                  << passed << std::endl; 
        std::cout << "Failed frames:                    " 
                  << received - passed << std::endl; 
    }

    int deframer_impl::passed_checks() const
    {
        return passed; 
    }

  } /* namespace Spread */
} /* namespace gr */

