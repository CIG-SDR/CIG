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
#include "sync_impl.h"

namespace gr {
  namespace Spread {

    sync::sptr
    sync::make()
    {
      return gnuradio::get_initial_sptr
        (new sync_impl());
    }

    /*
     * The private constructor
     */
    sync_impl::sync_impl()
      : gr::sync_block("sync",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(char))), 
        d_offset(0), 
        buffer_length(0),
        buffer(NULL), 
        
        // 13-bit Barker Code
        sync_bits {1, 1, 1, 1, 
                   1, 0, 0, 1, 
                   1, 0, 1, 0, 
                   1}, 

        zero_pad  {0}
    {
        message_port_register_in(pmt::mp("in")); 

        set_msg_handler(pmt::mp("in"), boost::bind(&sync_impl::pmt_in, this, _1)); 
    }

    /*
     * Our virtual destructor.
     */
    sync_impl::~sync_impl()
    {
        delete [] buffer; 
    }

    void
    sync_impl::pmt_in(pmt::pmt_t msg) 
    {
        msg_queue.push(msg); 
    }

    int
    sync_impl::work (int noutput_items,
                  gr_vector_const_void_star &input_items,
                  gr_vector_void_star &output_items)
    {
        char *out = (char *) output_items[0];

        // Insert the synchronization bits
        if (msg_queue.size() > 0 && d_offset == 0) { 

            pmt::pmt_t msg = msg_queue.front(); 

            msg_queue.pop(); 

            if (pmt::is_blob(msg)) { 
                int blob_size = pmt::blob_length(msg); 
                const char *blob = static_cast<const char *>(pmt::blob_data(msg)); 

                // Create buffer to hold samples 
                buffer_length = sizeof(zero_pad) + sizeof(sync_bits) + 8 * blob_size; 
                buffer = new char[buffer_length]; 

                std::cout << "Frame sync bytes out: " << buffer_length << std::endl; 

                // Copy the frame into the buffer
                std::memcpy(buffer, zero_pad, sizeof(zero_pad)); 
                std::memcpy(buffer + sizeof(zero_pad), sync_bits, sizeof(sync_bits)); 

                // Unpack the blob bytes to the buffer
                unpack_bytes(buffer + sizeof(zero_pad) + sizeof(sync_bits), blob, blob_size); 
            } 
        }

        // Output as much of the buffer as possible this work call
        int nprocess = std::min(buffer_length - d_offset, noutput_items); 

        for (int i = 0; i < nprocess; i++)  
            out[i] = buffer[i]; 

        d_offset += nprocess; 

        if (d_offset == buffer_length) {

            delete [] buffer; 
            buffer = NULL; 
            buffer_length = 0; 

            d_offset = 0; 
        }

        return nprocess;

   }

  } /* namespace Spread */
} /* namespace gr */

