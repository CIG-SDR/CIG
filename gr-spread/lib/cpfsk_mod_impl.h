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

#ifndef INCLUDED_SPREAD_CPFSK_MOD_IMPL_H
#define INCLUDED_SPREAD_CPFSK_MOD_IMPL_H

#include <Spread/cpfsk_mod.h>

namespace gr {
  namespace Spread {

    class cpfsk_mod_impl : public cpfsk_mod
    {
     private:
      const int samp_sym;
      int d_offset; 
      double phase; 

     public:
      cpfsk_mod_impl(const int samples_per_symbol);
      ~cpfsk_mod_impl();

      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_CPFSK_MOD_IMPL_H */

