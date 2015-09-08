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


#ifndef INCLUDED_SPREAD_SYNC_H
#define INCLUDED_SPREAD_SYNC_H

#include <Spread/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Spread {

    /*!
     * \brief Takes a PDU, zero pads it, and inserts a bit sync pattern
     * \ingroup Spread
     *
     */
    class SPREAD_API sync : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<sync> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Spread::sync.
       *
       * To avoid accidental use of raw pointers, Spread::sync's
       * constructor is in a private implementation
       * class. Spread::sync::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_SYNC_H */

