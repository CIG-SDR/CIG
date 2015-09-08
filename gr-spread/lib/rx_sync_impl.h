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

#ifndef INCLUDED_SPREAD_RX_SYNC_IMPL_H
#define INCLUDED_SPREAD_RX_SYNC_IMPL_H

#include <gnuradio/filter/fir_filter.h> 
#include <Spread/rx_sync.h>

#include "common.h"
#include "format.h"

namespace gr {
  namespace Spread {

    class rx_sync_impl : public rx_sync
    {
     private:
      const float _threshold; 

      int d_offset; 
      float d_correlator; 

      char sync_bits[13], zero_pad[16];
      char header_bytes[4]; 

      bool frame_found; 
      frame_header header; 

      char *frame_data; 

      filter::kernel::fir_filter_fff * filter_object; 

      std::vector<float> buffer; 

     public:
      rx_sync_impl(const float threshold);
      ~rx_sync_impl();

      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_RX_SYNC_IMPL_H */

