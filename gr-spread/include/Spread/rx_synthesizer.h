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


#ifndef INCLUDED_SPREAD_RX_SYNTHESIZER_H
#define INCLUDED_SPREAD_RX_SYNTHESIZER_H

#include <Spread/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace Spread {

    /*!
     * \brief Despreads incoming complex baseband samples.  
     * \ingroup Spread
     *
     */
    class SPREAD_API rx_synthesizer : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<rx_synthesizer> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Spread::rx_synthesizer.
       *
       * To avoid accidental use of raw pointers, Spread::rx_synthesizer's
       * constructor is in a private implementation
       * class. Spread::rx_synthesizer::make is the public interface for
       * creating new instances.
       */
      static sptr make(const int samples_per_hop, 
                       const int samples_per_symbol, 
                       const double sample_rate, 
                       const double high_freq,
                       const double transition, 
                       const double energy_threshold, 
                       const std::vector<unsigned char> gen, 
                       const std::vector<unsigned char> init);
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_RX_SYNTHESIZER_H */

