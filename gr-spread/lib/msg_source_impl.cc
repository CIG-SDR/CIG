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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/block_detail.h>

#include <string>
#include <stdlib.h>

#include "msg_source_impl.h"

namespace gr {
  namespace Spread {

    msg_source::sptr
    msg_source::make(const int num_messages, const size_t len)
    {
      return gnuradio::get_initial_sptr
        (new msg_source_impl(num_messages, len));
    }

    /*
     * The private constructor
     */
    msg_source_impl::msg_source_impl(const int num_messages, const size_t len)
      : gr::block("msg_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)), 
        _len(len), 
        limit(num_messages), 
        published(0), 
        done(false)
    {
       message_port_register_out(pmt::mp("out")); 
       _thread = boost::shared_ptr<boost::thread> (new boost::thread(boost::bind(&msg_source_impl::work, this))); 
    }

    msg_source_impl::~msg_source_impl()
    {
        _thread->interrupt();
        _thread->join(); 
    }

    void msg_source_impl::work()
    {
       while (!done) {
           boost::this_thread::sleep(boost::posix_time::milliseconds(100)); 

           if (published == limit) { 
               message_port_pub(pmt::mp("out"), pmt::PMT_EOF); 
               detail().get()->set_done(true); 
               done = true; 
               return; 
           }

           message_port_pub(pmt::mp("out"), gen_string(_len));  
           published++; 
       } 
    }

    pmt::pmt_t msg_source_impl::gen_string(const size_t len)
    {
        std::string random_string(len, 'x'); 
        static const char set[] = 
           "0123456789"
           "ABCDEFGHIJKLMNOPOQRSTUVWXYZ"
           "abcdefghijklmnopqrstuvwxyz"; 

        for (int i = 0; i < len; i++)
            random_string[i] = set[rand() % (sizeof(set) - 1)]; 
         
        return pmt::string_to_symbol(random_string);  
    }

  } /* namespace Spread */
} /* namespace gr */

