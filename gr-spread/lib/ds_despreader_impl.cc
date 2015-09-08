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
#include "ds_despreader_impl.h"

namespace gr {
  namespace Spread {

    ds_despreader::sptr
    ds_despreader::make(const int samples_per_chip, 
                        const int samples_between, 
                        const float threshold, 
                        const polynomial gen, 
                        const polynomial init)
    {
      return gnuradio::get_initial_sptr
        (new ds_despreader_impl(samples_per_chip, samples_between, threshold, gen, init));
    }

    /*
     * The private constructor
     */
    ds_despreader_impl::ds_despreader_impl(const int samples_per_chip, 
                                           const int samples_between, 
                                           const float threshold, 
                                           const polynomial gen, 
                                           const polynomial init)
      : gr::block("ds_despreader",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))), 
        _samp_chip(samples_per_chip), 
        _size(init.size()), 
        _threshold(threshold), 
        d_correlator(0), 
        d_clock(0), 
        d_offset(0) 
    {
        code = new LFSR(gen, init);

        bool repeated = false; 

        // Create a record of our PN sequence
        int first = code->getFrequency();
        while (!repeated) { 
            for (int j = 0; j < _samp_chip; j++) 
               chip_sequence.push_back(2*code->getRegister(_size - 1) - 1);  
            
            code->update(); 

            if (code->getFrequency() == first) 
                repeated = true; 
        }

        code->reset(); 

        // One-time reverse for the acquisition correlation taps
        std::reverse(chip_sequence.begin(), chip_sequence.end()); 

        acquire_filter = new filter::kernel::fir_filter_fff(1, chip_sequence); 
        buffer = std::vector<float>(acquire_filter->ntaps(), 0); 

        std::cout << "Acquisition ntaps: " << acquire_filter->ntaps() << std::endl; 

        // Reverse back to despread the sequence
        std::reverse(chip_sequence.begin(), chip_sequence.end()); 
    }

    /*
     * Our virtual destructor.
     */
    ds_despreader_impl::~ds_despreader_impl()
    {
        delete code; 
    }

    void 
    ds_despreader_impl::forecast(int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items; 
    }

    int
    ds_despreader_impl::general_work(int noutput_items,
              gr_vector_int &ninput_items, 
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        float filtered_sample = 0; 

        for (int i = 0; i < noutput_items; i++) { 

            buffer.push_back(in[i].real()); 
            buffer.erase(buffer.begin()); 

            filtered_sample = acquire_filter->filter(&buffer[0]); 

            if (d_correlator < filtered_sample) { 
                d_correlator = filtered_sample; 
                std::cout << "Maximum correlation: " << d_correlator << std::endl; 
            }

            if (filtered_sample >= _threshold) { 

                d_clock = 0; 
                d_offset = 0; 
                d_correlator = 0; 

                std::cout << "Found correlation!" << std::endl; 

                consume_each(i + 1); 
                return i + 1; 
            }

            int chip = chip_sequence[d_clock % chip_sequence.size()]; 

            gr_complex random_output(chip, 0); 
            out[i] = random_output * in[i]; 

            d_clock++;
            d_offset++; 
        }

        consume_each(noutput_items); 
        return noutput_items; 
    }

  } /* namespace Spread */
} /* namespace gr */

