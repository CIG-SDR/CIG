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


#ifndef INCLUDED_SPREAD_DEFRAMER_H
#define INCLUDED_SPREAD_DEFRAMER_H

#include <Spread/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace Spread {

    /*!
     * \brief This block extracts the data and performs an error check 
     * \ingroup Spread
     *
     */
    class SPREAD_API deframer : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<deframer> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Spread::deframer.
       *
       * To avoid accidental use of raw pointers, Spread::deframer's
       * constructor is in a private implementation
       * class. Spread::deframer::make is the public interface for
       * creating new instances.
       */
      static sptr make(int test);

      virtual int passed_checks() const = 0; 
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_DEFRAMER_H */

