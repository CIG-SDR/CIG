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

#ifndef INCLUDED_SPREAD_MSG_SOURCE_IMPL_H
#define INCLUDED_SPREAD_MSG_SOURCE_IMPL_H

#include <Spread/msg_source.h>

namespace gr {
  namespace Spread {

    class msg_source_impl : public msg_source
    {
     private:
      boost::shared_ptr<boost::thread> _thread; 
      const size_t _len;
      int limit, published; 

      bool done; 

     public:
      msg_source_impl(const int num_messages, const size_t len);
      ~msg_source_impl(); 

      void work(); 

      pmt::pmt_t gen_string(const size_t len);  
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_MSG_SOURCE_IMPL_H */

