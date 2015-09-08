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

#ifndef INCLUDED_SPREAD_SYNTHESIZER_IMPL_H
#define INCLUDED_SPREAD_SYNTHESIZER_IMPL_H

#include <Spread/synthesizer.h>

#include "code_generator.h"

namespace gr {
  namespace Spread {

    class synthesizer_impl : public synthesizer
    {
     private:
      const int _samp_hop; 
      const int _size; 
      const double _sample_rate; 

      std::vector<int> pattern_sequence; 
      std::vector<double> freq_set; 

      LFSR * code; 

      int d_delay; 
      int d_clock;
      int d_offset; 

     public:
      synthesizer_impl(const int samples_per_hop,
                       const int delay, 
                       const double sample_rate, 
                       const polynomial gen, 
                       const polynomial init);

      ~synthesizer_impl();

      void generate_hop_freqs(); 
      
      gr_complex transform(double freq, gr_complex sample); 

      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_SYNTHESIZER_IMPL_H */

