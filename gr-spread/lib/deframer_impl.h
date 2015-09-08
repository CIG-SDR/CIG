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

#ifndef INCLUDED_SPREAD_DEFRAMER_IMPL_H
#define INCLUDED_SPREAD_DEFRAMER_IMPL_H

#include <Spread/deframer.h>

namespace gr {
  namespace Spread {

    class deframer_impl : public deframer
    {
     private:
      char *frame_data;
      int frame_len; 
      int received, passed; 

     public:
      deframer_impl(int test);

      void pmt_in(pmt::pmt_t msg);  

      void parse_frame(const char *pdu_data, const int pdu_len); 
      void print(const uint8_t type, const uint8_t length, const bool result); 

      int passed_checks() const; 
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_DEFRAMER_IMPL_H */

