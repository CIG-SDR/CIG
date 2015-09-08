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

#include "qa_cpfsk.h"
#include <Spread/cpfsk_mod.h>
#include <Spread/cpfsk_demod.h>
#include <gnuradio/blocks/vector_source_b.h>
#include <gnuradio/blocks/vector_sink_b.h>
#include <gnuradio/blocks/head.h>

#include <gnuradio/top_block.h>

typedef gr::blocks::vector_source_b::sptr vec_source_sptr; 
typedef gr::blocks::vector_sink_b::sptr vec_sink_sptr; 
typedef gr::blocks::head::sptr head_sptr; 

namespace gr {
  namespace Spread {

    void
    qa_cpfsk::t1()
    {
        std::cout << "Test 1: CPFSK Mod / Demod" << std::endl; 

        top_block_sptr tb = make_top_block("CPFSK mod/demod test"); 

        int samp_sym = 4; 

        unsigned char input_array[] = {0, 0, 0, 0, 
                                       1, 1, 1, 1, 
                                       1, 0, 1, 0, 
                                       1, 0, 0, 1, 
                                       1, 0, 0, 0, 
                                       0, 1, 1, 1, 
                                       1, 0, 1, 1, 
                                       1, 0, 0, 0}; 
                                       

        std::vector<unsigned char> input(input_array, input_array + sizeof(input_array)); 
        std::vector<unsigned char> output; 

        vec_source_sptr vector_source = gr::blocks::vector_source_b::make(input); 

        cpfsk_mod::sptr modulator = cpfsk_mod::make(samp_sym); 
        cpfsk_demod::sptr demodulator = cpfsk_demod::make(samp_sym); 

        vec_sink_sptr vector_sink = gr::blocks::vector_sink_b::make(); 

        tb->connect(vector_source, 0, modulator, 0);
        tb->connect(modulator, 0, demodulator, 0); 
        tb->connect(demodulator, 0, vector_sink, 0); 

        tb->start(); 
        boost::this_thread::sleep(boost::posix_time::seconds(1)); 
        output = vector_sink->data();
        tb->stop();

        // Print and assert results
        std::cout << "Input  = {"; 
        for (int i = 0; i < sizeof(input_array); i++)
            std::cout << (int) input_array[i] << " "; 
        std::cout << "}" << std::endl; 

        std::cout << "Output = {"; 
        for (int i = 0; i < output.size(); i++)
            std::cout << (int) output[i] << " "; 
        std::cout << "}" << std::endl; 

        for (int i = 0; i < output.size(); i++) { 
            CPPUNIT_ASSERT_EQUAL(input[i], output[i]); 
        }
        
        std::cout << "CPFSK Mod / Demod test passed!!" << std::endl; 
    }

  } /* namespace Spread */
} /* namespace gr */

