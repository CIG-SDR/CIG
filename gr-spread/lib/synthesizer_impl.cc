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
#include "synthesizer_impl.h"

namespace gr {
  namespace Spread {

    synthesizer::sptr
    synthesizer::make(const int samples_per_hop, 
                      const int delay, 
                      const double sample_rate, 
                      const polynomial gen, 
                      const polynomial init)
    {
      return gnuradio::get_initial_sptr
        (new synthesizer_impl(samples_per_hop, 
                              delay, 
                              sample_rate, 
                              gen, 
                              init));
    }

    /*
     * The private constructor
     */
    synthesizer_impl::synthesizer_impl(const int samples_per_hop, 
                                       const int delay, 
                                       const double sample_rate, 
                                       const polynomial gen, 
                                       const polynomial init)
      : gr::sync_block("synthesizer",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
        _samp_hop(samples_per_hop), 
        _sample_rate(sample_rate),
        _size(init.size()), 
        d_delay(delay), 
        d_clock(0), 
        d_offset(0)  
    {
        code = new LFSR(gen, init); 

        bool repeated = false; 
        while (!repeated) { 
            for (int j = 0; j < _samp_hop; j++) 
                pattern_sequence.push_back(code->getFrequency()); 

            code->update(); 

            if (code->getFrequency() == pattern_sequence[0]) 
                repeated = true; 
        }

        code->reset(); 

        generate_hop_freqs(); 
    }

    /*
     * Our virtual destructor.
     */
    synthesizer_impl::~synthesizer_impl()
    {
        delete code; 
    }

    void synthesizer_impl::generate_hop_freqs()
    {
        // Given the total size of the incoming vectors 
        // generate a number of frequencies of that size 
        // that fit within the specified bandwidth (sampling rate)  
        int nfreqs = std::pow(2, _size); 

        double bandwidth = _sample_rate; 
        double channel_width = bandwidth / nfreqs; 
        double inc = channel_width / 2;

        for (int i = 0; i < nfreqs; i++) { 
            freq_set.push_back(inc + i*channel_width - bandwidth / 2); 
            std::cout << "Center Freq " << i << ": " << inc + i*channel_width - bandwidth / 2 << std::endl; 
        }
        
    }

    gr_complex synthesizer_impl::transform(double freq, gr_complex sample) 
    {
        gr_complex phasor = static_cast<gr_complex> (std::polar(1.0, 2 * M_PI * freq * d_offset / _sample_rate));  
        gr_complex result = phasor * sample; 

        if (d_offset == _sample_rate) 
            d_offset = 0; 

        return result; 
    }

    int
    synthesizer_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *sample_input = (const gr_complex *) input_items[0]; 
        gr_complex *out = (gr_complex *) output_items[0];

        // Loop through each input item pair
        // (code generator vector, complex sample) 
        // Map the code vector to a number (hence frequency) 

        // Spread the incoming baseband complex samples
        for (int i = 0; i < noutput_items; i++) { 
            int selected_freq = pattern_sequence[d_clock % pattern_sequence.size()];  
            out[i] = transform(freq_set[selected_freq], sample_input[i]); 

            if (d_delay > 0) { 
                d_clock--; 
                d_delay--; 

            } else if (d_delay < 0) {
                d_clock += d_delay;  
                d_delay = 0; 
            }

            d_clock++; 
            d_offset++;
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace Spread */
} /* namespace gr */

