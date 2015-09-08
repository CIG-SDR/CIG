/* -*- c++ -*- */
/* 
 * Copyright 2014 Paul David <pudavid@vt.edu> * 
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

#ifndef INCLUDED_SPREAD_RX_SYNTHESIZER_IMPL_H
#define INCLUDED_SPREAD_RX_SYNTHESIZER_IMPL_H

#include <Spread/rx_synthesizer.h>

#include <gnuradio/filter/firdes.h>
#include <gnuradio/filter/fir_filter.h>

#include "code_generator.h"

namespace gr {
  namespace Spread {

    class rx_synthesizer_impl : public rx_synthesizer
    {
     private:
      const int _samp_hop; 
      const int _samp_sym; 
      const int _size; 

      const double _transition; 
      const double _sample_rate;       
      const double _energy_threshold; 

      int d_offset, d_clock; 
      int clock_error; 

      float d_energy; 

      enum modes {SYNC, TRACK} d_mode; 

      std::vector<int> pattern_sequence; 
      std::vector<double> freq_set; 

      std::vector<gr_complex> acq_buffer; 
      std::vector<gr_complex> track_buffer; 

      filter::kernel::fir_filter_ccf * track_filter; 

      LFSR * code; 

     public:
      rx_synthesizer_impl(const int samples_per_hop, 
                          const int samples_per_symbol, 
                          const double sample_rate, 
                          const double high_freq,
                          const double transition, 
                          const double energy_threshold, 
                          const polynomial gen, 
                          const polynomial init); 

      ~rx_synthesizer_impl();

      void generate_hop_freqs(); 
      void add_tag(uint64_t item); 

      gr_complex transform(double freq, gr_complex sample); 

      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_RX_SYNTHESIZER_IMPL_H */

