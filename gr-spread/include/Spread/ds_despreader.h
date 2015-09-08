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


#ifndef INCLUDED_SPREAD_DS_DESPREADER_H
#define INCLUDED_SPREAD_DS_DESPREADER_H

#include <Spread/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace Spread {

    /*!
     * \brief Despreads the incoming direct-sequence signal. 
     * \ingroup Spread
     *
     */
    class SPREAD_API ds_despreader : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<ds_despreader> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of Spread::ds_despreader.
       *
       * To avoid accidental use of raw pointers, Spread::ds_despreader's
       * constructor is in a private implementation
       * class. Spread::ds_despreader::make is the public interface for
       * creating new instances.
       */
      static sptr make(const int samples_per_chip, 
                       const int samples_between,
                       const float threshold, 
                       const std::vector<unsigned char> gen, 
                       const std::vector<unsigned char> init);
    };

  } // namespace Spread
} // namespace gr

#endif /* INCLUDED_SPREAD_DS_DESPREADER_H */

