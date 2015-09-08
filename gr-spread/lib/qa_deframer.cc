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


#include <gnuradio/attributes.h>
#include <cppunit/TestAssert.h>
#include <iostream> 

#include "qa_deframer.h"
#include <Spread/framer.h>
#include <Spread/deframer.h>
#include <Spread/msg_source.h>

#include <gnuradio/top_block.h>

namespace gr {
  namespace Spread {

    void
    qa_deframer::t1()
    {
        std::cout << "Test 1: Framer / Deframer" << std::endl; 
        int expected = 10, passed = 0; 

        top_block_sptr tb = make_top_block("Framer/deframer test"); 
       
        msg_source::sptr msg_src = msg_source::make(expected, 10); 
        framer::sptr framer_block = framer::make(0); 
        deframer::sptr deframer_block = deframer::make(0); 

        tb->msg_connect(msg_src, pmt::mp("out"), framer_block, pmt::mp("in"));
        tb->msg_connect(framer_block, pmt::mp("out"), deframer_block, pmt::mp("in"));
        
        tb->start(); 
        boost::this_thread::sleep(boost::posix_time::seconds(1)); 
        passed = deframer_block->passed_checks(); 
        tb->stop(); 

        CPPUNIT_ASSERT_EQUAL(expected, passed);  

        std::cout << "Framer / Deframer test passed!!" << std::endl; 
    }

  } /* namespace Spread */
} /* namespace gr */

