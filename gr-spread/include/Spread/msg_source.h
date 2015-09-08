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


#ifndef INCLUDED_SPREAD_MSG_SOURCE_H
#define INCLUDED_SPREAD_MSG_SOURCE_H

#include <Spread/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace Spread {

    /*!
     * \brief This block acts as a random message source, 
     * generating some finite number of messages. 
     * \ingroup Spread
     *
     */
    class SPREAD_API msg_source : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<msg_source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Spread::msg_source.
       *
       * To avoid accidental use of raw pointers, Spread::msg_source's
       * constructor is in a private implementation
       * class. Spread::msg_source::make is the public interface for
       * creating new instances.
       */
      static sptr make(const int num_messages, const size_t len);
       
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_MSG_SOURCE_H */

