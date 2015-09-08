/* -*- c++ -*- */

#define SPREAD_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "Spread_swig_doc.i"

%{
#include "Spread/synthesizer.h"
#include "Spread/framer.h"
#include "Spread/deframer.h"
#include "Spread/sync.h"
#include "Spread/cpfsk_mod.h"
#include "Spread/cpfsk_demod.h"
#include "Spread/msg_source.h"
#include "Spread/rx_synthesizer.h"
#include "Spread/ds_spreader.h"
#include "Spread/ds_despreader.h"
#include "Spread/preamble_bb.h"
#include "Spread/preamble_cc.h"
#include "Spread/rx_sync.h"
%}


%include "Spread/synthesizer.h"
GR_SWIG_BLOCK_MAGIC2(Spread, synthesizer);
%include "Spread/framer.h"
GR_SWIG_BLOCK_MAGIC2(Spread, framer);
%include "Spread/deframer.h"
GR_SWIG_BLOCK_MAGIC2(Spread, deframer);


%include "Spread/sync.h"
GR_SWIG_BLOCK_MAGIC2(Spread, sync);
%include "Spread/cpfsk_mod.h"
GR_SWIG_BLOCK_MAGIC2(Spread, cpfsk_mod);
%include "Spread/cpfsk_demod.h"
GR_SWIG_BLOCK_MAGIC2(Spread, cpfsk_demod);
%include "Spread/msg_source.h"
GR_SWIG_BLOCK_MAGIC2(Spread, msg_source);
%include "Spread/rx_synthesizer.h"
GR_SWIG_BLOCK_MAGIC2(Spread, rx_synthesizer);

%include "Spread/ds_spreader.h"
GR_SWIG_BLOCK_MAGIC2(Spread, ds_spreader);
%include "Spread/ds_despreader.h"
GR_SWIG_BLOCK_MAGIC2(Spread, ds_despreader);
%include "Spread/preamble_bb.h"
GR_SWIG_BLOCK_MAGIC2(Spread, preamble_bb);
%include "Spread/preamble_cc.h"
GR_SWIG_BLOCK_MAGIC2(Spread, preamble_cc);
%include "Spread/rx_sync.h"
GR_SWIG_BLOCK_MAGIC2(Spread, rx_sync);
