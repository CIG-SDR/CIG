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
#include "qa_code_generator.h"
#include <iostream> 

#include "common.h"
#include "code_generator.h"

namespace gr {
  namespace Spread {

    void
    qa_code_generator::t1()
    {
        std::cout << "Test 1: LFSR pattern 1" << std::endl; 

        // Create expected register states
        bit bits[7][3] = { {1, 1, 1}, 
                           {1, 0, 1}, 
                           {1, 0, 0}, 
                           {0, 1, 0}, 
                           {0, 0, 1}, 
                           {1, 1, 0}, 
                           {0, 1, 1} }; 

        bit gen_bits[] = {1, 1, 0, 1}; 
        polynomial gen(gen_bits, gen_bits + 4); 
        polynomial init(3, 1); 

        LFSR * srs = new LFSR(gen, init); 

        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 3; j++)   
                CPPUNIT_ASSERT_EQUAL(srs->getRegister(j), bits[i][j]); 

            srs->update(); 
        }

        delete srs; 

        std::cout << "LFSR pattern 1 passed!!" << std::endl; 
    }

    void
    qa_code_generator::t2()
    {
        std::cout << "Test 2: LFSR pattern 2" << std::endl; 

        // Create expected register states
        bit bits[8][3] = { {0, 0, 1}, 
                           {1, 1, 0}, 
                           {0, 1, 1}, 
                           {1, 1, 1}, 
                           {1, 0, 1}, 
                           {1, 0, 0}, 
                           {0, 1, 0},
                           {0, 0, 1} }; 

        bit gen_bits[] = {1, 1, 0, 1}; 
        polynomial gen(gen_bits, gen_bits + 4); 

        bit init_bits[] = {0, 0, 1}; 
        polynomial init(init_bits, init_bits + 3); 

        LFSR * srs = new LFSR(gen, init); 

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 3; j++)  
                CPPUNIT_ASSERT_EQUAL(srs->getRegister(j), bits[i][j]); 

            srs->update(); 
        }

        delete srs; 

        std::cout << "LFSR pattern 2 passed!!" << std::endl; 
    }

  } /* namespace Spread */
} /* namespace gr */

