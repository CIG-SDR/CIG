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


#ifndef INCLUDED_SPREAD_CPFSK_MOD_H
#define INCLUDED_SPREAD_CPFSK_MOD_H

#include <Spread/api.h>
#include <gnuradio/sync_interpolator.h>

namespace gr {
  namespace Spread {

    /*!
     * \brief This block modulates incoming bits as a CPFSK signal 
     * \ingroup Spread
     *
     */
    class SPREAD_API cpfsk_mod : virtual public gr::sync_interpolator
    {
     public:
      typedef boost::shared_ptr<cpfsk_mod> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Spread::cpfsk_mod.
       *
       * To avoid accidental use of raw pointers, Spread::cpfsk_mod's
       * constructor is in a private implementation
       * class. Spread::cpfsk_mod::make is the public interface for
       * creating new instances.
       */
      static sptr make(const int samples_per_symbol);
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_CPFSK_MOD_H */

