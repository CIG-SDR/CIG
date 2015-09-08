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

#include "framer_impl.h"

#include <boost/crc.hpp>
#include <stdexcept> 

namespace gr {
  namespace Spread {

    framer::sptr
    framer::make(int test)
    {
      return gnuradio::get_initial_sptr
        (new framer_impl(test));
    }

    /*
     * The private constructor
     */
    framer_impl::framer_impl(int test)
      : gr::block("framer",
              gr::io_signature::make(0, 0, 0), 
              gr::io_signature::make(0, 0, 0)), 
        frame_data(NULL), frame_len(0), nseq(0) 
    { 
        message_port_register_out(pmt::mp("out")); 
        message_port_register_in(pmt::mp("in")); 

        set_msg_handler(pmt::mp("in"), boost::bind(&framer_impl::pmt_in, this, _1));
    }

    void framer_impl::pmt_in(pmt::pmt_t msg)
    {
        const char   *pdu; 
        size_t       msg_len; 

        if (pmt::is_symbol(msg)) { 

            // Read the string and its length
            std::string data; 
            data = pmt::symbol_to_string(msg);
            msg_len = data.length(); 
            pdu = data.data(); 

        } else if (pmt::is_eof_object(msg)) {

            message_port_pub(pmt::mp("out"), pmt::PMT_EOF); 
            detail().get()->set_done(true); 
            return; 

        } else { 

            throw std::invalid_argument("Framer only accepts strings"); 
        }

        // Create a frame  
        create_frame(pdu, msg_len); 
        pmt::pmt_t blob = pmt::make_blob(frame_data, frame_len); 

        // Publish the message
        message_port_pub(pmt::mp("out"), blob); 

        delete[] frame_data; 
        frame_data = NULL; 
    }

    void framer_impl::create_frame(const char *pdu_data, const int pdu_len)
    {
        // Create the frame header 
        // TODO: Create some higher MAC layer for multiple radios

        frame_header header; 

        header.type = 0x00;
        header.dst = 0x00; 
        header.src = 0x00; 

        header.len = pdu_len; 

        // Copy the data into the buffer
        frame_len = 4 + pdu_len + 4; // header + data + fcs 
        frame_data = new char[frame_len]; 

        std::memcpy(frame_data, &header, sizeof(frame_header)); 
        std::memcpy(frame_data + 4, pdu_data, pdu_len); 
         
        // Create the checksum 
        boost::crc_32_type data; 
        data.process_bytes(frame_data, 4 + pdu_len); 

        uint32_t fcs = data.checksum(); 
        /*
        std::cout << "======================================" << std::endl; 
        std::cout << "FRAMER OUT FCS:    " << fcs << std::endl;
        std::cout << "FRAMER OUT LENGTH: " << pdu_len << std::endl; 
        */ 

        std::memcpy(frame_data + 4 + pdu_len, &fcs, 4);  
    }


  } /* namespace Spread */
} /* namespace gr */

