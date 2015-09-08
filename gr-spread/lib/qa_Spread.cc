/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * This class gathers together all the test cases for the gr-filter
 * directory into a single test suite.  As you create new test cases,
 * add them here.
 */

#include "qa_Spread.h"
#include "qa_synthesizer.h"
#include "qa_deframer.h"

#include "qa_code_generator.h"
#include "qa_sync.h"
#include "qa_cpfsk.h"

CppUnit::TestSuite *
qa_Spread::suite()
{
  CppUnit::TestSuite *s = new CppUnit::TestSuite("Spread");
  s->addTest(gr::Spread::qa_synthesizer::suite());
  s->addTest(gr::Spread::qa_deframer::suite());

  // s->addTest(gr::Spread::qa_dpsk_mod::suite());
  // s->addTest(gr::Spread::qa_dpsk_demod::suite());
  s->addTest(gr::Spread::qa_code_generator::suite());
  // s->addTest(gr::Spread::qa_sync::suite());
  s->addTest(gr::Spread::qa_cpfsk::suite());

  return s;
}
