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
#include "@NAME_IMPL@.h"

namespace gr {
  namespace Spread {

    @NAME@::sptr
    @NAME@::make(const int samples_between, 
                 std::vector<gr_complex> pattern)
    {
      return gnuradio::get_initial_sptr
        (new @NAME_IMPL@(samples_between, pattern));
    }

    /*
     * The private constructor
     */
    @NAME_IMPL@::@NAME_IMPL@(const int samples_between,  
                             std::vector<gr_complex> pattern)
      : gr::block("preamble",
              gr::io_signature::make(1, 1, sizeof(@TYPE@)),
              gr::io_signature::make(1, 1, sizeof(@TYPE@))), 
        _nsamples(samples_between), 
        _pattern(pattern), 
        d_symbols_left(0), 
        d_offset(0) 
    {
    }

    /*
     * Our virtual destructor.
     */
    @NAME_IMPL@::~@NAME_IMPL@()
    {
    }

    void
    @NAME_IMPL@::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items; 
    }

    int
    @NAME_IMPL@::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        int ninput = ninput_items[0];

        const @TYPE@ *in = (const @TYPE@ *) input_items[0];
        @TYPE@ *out = (@TYPE@ *) output_items[0];

        int noutput = std::min(ninput, noutput_items); 
        int npattern = _pattern.size(); 
        int count = 0; 

        if (d_symbols_left < npattern) {

            // Insert training symbols for chip sequence
            int items_left = std::min(npattern - d_symbols_left, noutput_items); 
            for (int i = 0; i < items_left; i++) { 
                out[count] = _pattern[d_symbols_left].real(); 

                // std::cout << "Preamble symbol: " << out[count] << std::endl; 

                d_symbols_left++;
                count++;
            }

            consume_each(0); 

        } else { 

            for (int i = 0; i < noutput; i++) { 

                // Copy the input symbols 
                out[count] = in[i];
                
                /*
                std::cout << "Copy symbol: " << out[count] << std::endl; 
                std::cout << "Count: " << count << " d_offset: " << d_offset << std::endl; 
                */

                d_offset++;
                count++;

                if (d_offset >= _nsamples) { 
                    d_symbols_left = 0; 
                    d_offset = 0; 
                    consume_each(count); 
                    return count; 
                }
            }

            consume_each(noutput); 
        }

        // Tell runtime system how many output items we produced.
        return count;
    }

  } /* namespace Spread */
} /* namespace gr */

