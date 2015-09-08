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

#include "qa_synthesizer.h"
#include "common.h"

#include <Spread/synthesizer.h>
#include <Spread/rx_synthesizer.h>
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
    qa_synthesizer::t1()
    {
        std::cout << "Test 1: Closed-loop FHSS test" << std::endl; 

        top_block_sptr tb = make_top_block("Simple FHSS test"); 

        int samp_sym = 16; 
        int samp_hop = 32; 
        double sample_rate = 100e3; 
       
        unsigned char input_array[] = {0, 0, 1, 0, 
                                       1, 0, 0, 1, 
                                       1, 0, 0, 0, 
                                       0, 1, 1, 1, 
                                       1, 0, 1, 1, 
                                       1, 0, 1, 0, 
                                       1, 0, 1, 1}; 

        std::vector<unsigned char> input(input_array, input_array + sizeof(input_array)); 
        std::vector<unsigned char> output; 

        bit gen_bits[] = {1, 1, 0, 1}; 
        polynomial gen(gen_bits, gen_bits + 4); 
        polynomial init(3, 1); 


        // Establish flowgraph names
        vec_source_sptr vector_source = gr::blocks::vector_source_b::make(input); 

        cpfsk_mod::sptr modulator = cpfsk_mod::make(samp_sym); 

        synthesizer::sptr tx_synth = synthesizer::make(samp_hop, 
                                                       0, 
                                                       sample_rate, 
                                                       gen, 
                                                       init);         

        rx_synthesizer::sptr rx_synth = rx_synthesizer::make(samp_hop,
                                                             samp_sym, 
                                                             sample_rate,
                                                             8000,
                                                             16000, 
                                                             0.6, 
                                                             gen,
                                                             init); 

        cpfsk_demod::sptr demodulator = cpfsk_demod::make(samp_sym); 

        vec_sink_sptr vector_sink = gr::blocks::vector_sink_b::make(); 

        // Connect the blocks
        tb->connect(vector_source, 0, modulator, 0);
        tb->connect(modulator, 0, tx_synth, 0); 
        tb->connect(tx_synth, 0, rx_synth, 0); 
        tb->connect(rx_synth, 0, demodulator, 0); 
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
        for (int i = 0; i < sizeof(input_array); i++)
            std::cout << (int) output[i] << " "; 
        std::cout << "}" << std::endl; 

        for (int i = 0; i < sizeof(input_array); i++) { 
            CPPUNIT_ASSERT_EQUAL(input[i], output[i]); 
        }

        std::cout << "Closed-loop FHSS test passed!!" << std::endl; 
    }

  } /* namespace Spread */
} /* namespace gr */

