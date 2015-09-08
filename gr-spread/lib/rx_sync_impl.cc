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
#include "rx_sync_impl.h"

namespace gr {
  namespace Spread {

    rx_sync::sptr
    rx_sync::make(const float threshold)
    {
      return gnuradio::get_initial_sptr
        (new rx_sync_impl(threshold));
    }

    /*
     * The private constructor
     */
    rx_sync_impl::rx_sync_impl(const float threshold)
      : gr::sync_block("rx_sync",
              gr::io_signature::make(1, 1, sizeof(bit)),
              gr::io_signature::make(0, 0, 0)),
        _threshold(threshold), 
        d_offset(0), 
        d_correlator(0), 
        frame_found(false), 

        sync_bits {1, 1, 1, 1, 
                   1, 0, 0, 1, 
                   1, 0, 1, 0, 
                   1}, 

        zero_pad  {0, 0, 0, 0,
                   0, 0, 0, 0,
                   0, 0, 0, 0,
                   0, 0, 0, 0}, 

        header_bytes {0, 0, 0, 0} 
    {
        message_port_register_out(pmt::mp("out")); 

        std::vector<float> taps(sizeof(sync_bits), 0); 
        for (int i = 0; i < sizeof(sync_bits); i++) 
            taps[i] = 2 * sync_bits[i] - 1; 

        std::reverse(taps.begin(), taps.end()); 

        filter_object = new filter::kernel::fir_filter_fff(1, taps); 

        buffer = std::vector<float>(filter_object->ntaps(), 0); 
        std::cout << "Correlation taps: " << filter_object->ntaps() << std::endl; 
    }

    /*
     * Our virtual destructor.
     */
    rx_sync_impl::~rx_sync_impl()
    {
        delete filter_object; 
    }

    int
    rx_sync_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const char *in = (const char *) input_items[0];

        if (frame_found == false) { 

            for (int i = 0; i < noutput_items; i++) { 

                buffer.push_back(2 * in[i] - 1); 
                buffer.erase(buffer.begin()); 

                float filtered_sample = filter_object->filter(&buffer[0]); 
                
                if (filtered_sample > _threshold) { 
                    frame_found = true; 

                    buffer.clear(); 
                    buffer = std::vector<float>(filter_object->ntaps(), 0); 

                    d_correlator = 0; 
                    d_offset = 0; 
                    return i + 1;    
                }

            }

        } else { 

            for (int i = 0; i < noutput_items; i++) { 
                // The incoming bytes are unpacked
                // Pack them into a header object
                if (d_offset < 32) { 
                    header_bytes[d_offset / 8] |= (in[i] << (d_offset % 8)); 
                } 

                // We're done with the header
                // and now know the length 
                if (d_offset == 31) {  
                    std::memcpy(&header, header_bytes, 4); 

                    frame_data = new char[4 + int(header.len) + 4](); // header + data + fcs
                    std::memcpy(frame_data, &header, 4);  

                } else if (d_offset >= 32 && d_offset < (64 + 8 * int(header.len))) { 

                    frame_data[d_offset / 8] |= (in[i] << (d_offset % 8)); 

                } 
                
                if (d_offset == (63 + 8 * int(header.len))) { 

                    // We completed the PDU - pack it into a blob
                    pmt::pmt_t blob = pmt::make_blob(frame_data, 4 + int(header.len) + 4); 
                    message_port_pub(pmt::mp("out"), blob); 

                    frame_found = false; 
                    return i + 1; 
                }

                d_offset++;     
            } 
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace Spread */
} /* namespace gr */

