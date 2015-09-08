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
#include "cpfsk_demod_impl.h"

namespace gr {
  namespace Spread {

    cpfsk_demod::sptr
    cpfsk_demod::make(const int samples_per_symbol)
    {
      return gnuradio::get_initial_sptr
        (new cpfsk_demod_impl(samples_per_symbol));
    }

    /*
     * The private constructor
     */
    cpfsk_demod_impl::cpfsk_demod_impl(const int samples_per_symbol)
      : gr::sync_decimator("cpfsk_demod",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)), 
              samples_per_symbol), 
        samp_sym(samples_per_symbol), d_offset(0)
    {
        sum[0] = gr_complex(0, 0);
        sum[1] = gr_complex(0, 0);  
    }

    /*
     * Our virtual destructor.
     */
    cpfsk_demod_impl::~cpfsk_demod_impl()
    {
    }

    int
    cpfsk_demod_impl::work (int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const int noutput = noutput_items; 
        const gr_complex *in = (const gr_complex *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        int count = 0; 

        for (int i = 0; i < noutput * samp_sym; i++) { 

            gr_complex jexp1(std::polar(1.0,  1 * M_PI * d_offset / samp_sym)); 
            gr_complex jexp2(std::polar(1.0, -1 * M_PI * d_offset / samp_sym)); 

            // Integrate over a symbol period 
            // using tone filters for binary CPFSK 
            sum[0] += in[i] * jexp1; 
            sum[1] += in[i] * jexp2; 

            // Compare the energy and make a hard decision on the bit
            if ((i + 1) % samp_sym == 0) {  
                 
                float compare = std::pow(std::abs(sum[0]), 2); 
                float second  = std::pow(std::abs(sum[1]), 2); 

                out[count] = (second > compare); 

                sum[0] = gr_complex(0, 0);
                sum[1] = gr_complex(0, 0); 
                count++; 
            }

            d_offset++;
        }

        return count;
    }

  } /* namespace Spread */
} /* namespace gr */

