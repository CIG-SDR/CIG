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
#include <cmath>
#include <complex>
#include "cpfsk_mod_impl.h"

namespace gr {
  namespace Spread {

    cpfsk_mod::sptr
    cpfsk_mod::make(const int samples_per_symbol)
    {
      return gnuradio::get_initial_sptr
        (new cpfsk_mod_impl(samples_per_symbol));
    }

    /*
     * The private constructor
     */
    cpfsk_mod_impl::cpfsk_mod_impl(const int samples_per_symbol)
      : gr::sync_interpolator("cpfsk_mod",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              samples_per_symbol), 
        samp_sym(samples_per_symbol), phase(0), d_offset(0)
    {
    }

    /*
     * Our virtual destructor.
     */
    cpfsk_mod_impl::~cpfsk_mod_impl()
    {
    }

    int
    cpfsk_mod_impl::work (int noutput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items)
    {
        const int noutput = noutput_items; 

        const unsigned char *in = (const unsigned char *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];
        
        for (int i = 0; i < noutput / samp_sym; i++) {

            for (int j = 0; j < samp_sym; j++) { 
                if (in[i] == 1) 
                    phase += M_PI / samp_sym;    
                else 
                    phase -= M_PI / samp_sym; 

                // phase wrapping 
                if (phase > 2*M_PI)
                   phase -= 2*M_PI; 
                else if (phase < -2*M_PI) 
                   phase += 2*M_PI;  

          
                out[samp_sym*i + j] = std::polar(0.999, phase); 
            }

        }

        return noutput;
    }

  } /* namespace Spread */
} /* namespace gr */

