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

#include "code_generator.h"

LFSR::LFSR(polynomial gen, polynomial init) : nreg(gen.size() - 1), 
                                              generator(gen), 
                                              _init(init) 
{
    if (nreg != init.size())
        throw std::runtime_error("Initalized wrong number of registers..."); 

    // Set the initial state 
    for (polynomial::iterator it = init.begin(); it != init.end(); ++it) { 
        state.push_back(*it); 
    }
}

void LFSR::update()
{
    bit out = state[nreg - 1]; 
    for (int i = nreg - 1; i >= 0; i--) {
        // Update each register value
        if (i == 0)   
            state[i] = out; 
        else  
            state[i] = state[i - 1] ^ (out & generator[i]); 
        
    }
}

bit LFSR::getRegister(int pos) const 
{
    return state[pos]; 
}

int LFSR::getFrequency() const 
{
    int freq = 0;  
    for (int i = 0; i < nreg; i++) 
        freq += state[i] * std::pow(2, i); 

    return freq; 
}

void LFSR::reset() 
{
    state.clear(); 

    // Set the initial state 
    for (polynomial::iterator it = _init.begin(); it != _init.end(); ++it) { 
        state.push_back(*it); 
    }
}


