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
#include "ds_spreader_impl.h"

namespace gr {
  namespace Spread {

    ds_spreader::sptr
    ds_spreader::make(const int samples_per_chip, 
                      const polynomial gen, 
                      const polynomial init)
    {
      return gnuradio::get_initial_sptr
        (new ds_spreader_impl(samples_per_chip, gen, init));
    }

    /*
     * The private constructor
     */
    ds_spreader_impl::ds_spreader_impl(const int samples_per_chip, 
                                       const polynomial gen, 
                                       const polynomial init) 
      : gr::sync_block("ds_spreader",
              gr::io_signature::make(1, 1, sizeof(bit)),
              gr::io_signature::make(1, 1, sizeof(bit))), 
        _samp_chip(samples_per_chip), 
        _size(init.size()), 
        d_clock(0), 
        d_offset(0) 
    {
        code = new LFSR(gen, init); 
    }

    /*
     * Our virtual destructor.
     */
    ds_spreader_impl::~ds_spreader_impl()
    {
        delete code; 
    }

    int
    ds_spreader_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const bit *in = (const bit *) input_items[0];
        bit *out = (bit *) output_items[0];

        for (int i = 0; i < noutput_items; i++) { 
            if ((d_clock % _samp_chip) == 0 && d_clock != 0) 
                code->update();

            bit random_output = code->getRegister(_size - 1); 

            out[i] = random_output ^ in[i];  

            d_clock++; 
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace Spread */
} /* namespace gr */

