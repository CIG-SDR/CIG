#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Direct-sequence spread spectrum
# Author: Paul David
# Generated: Tue Sep 16 17:23:12 2014
##################################################

from gnuradio import blocks
from gnuradio import channels
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import Spread
import wx

class dsss_sync(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Direct-sequence spread spectrum")

        ##################################################
        # Variables
        ##################################################
        self.variable_constellation_0 = variable_constellation_0 = digital.constellation_calcdist(([-1, 1]), ([0, 1]), 2, 1).base()
        self.samp_sym = samp_sym = 32
        self.samp_rate = samp_rate = 500e3
        self.samp_chip = samp_chip = 2
        self.init = init = 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
        self.generator = generator = 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_fftsink2_0_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="Despread Signal ",
        	peak_hold=False,
        )
        self.GridAdd(self.wxgui_fftsink2_0_0.win, 2, 3, 1, 2)
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="Received DS Signal",
        	peak_hold=False,
        )
        self.GridAdd(self.wxgui_fftsink2_0.win, 2, 1, 1, 2)
        self.digital_dxpsk_mod_0 = digital.dbpsk_mod(
        	samples_per_symbol=2,
        	excess_bw=0.35,
        	mod_code="gray",
        	verbose=False,
        	log=False)
        	
        self.digital_dxpsk_demod_0 = digital.dbpsk_demod(
        	samples_per_symbol=2,
        	excess_bw=0.35,
        	freq_bw=6.28/100.0,
        	phase_bw=6.28/100.0,
        	timing_bw=6.28/100.0,
        	mod_code="gray",
        	verbose=False,
        	log=False
        )
        self.channels_channel_model_0 = channels.channel_model(
        	noise_voltage=0.3,
        	frequency_offset=0.0,
        	epsilon=1.0,
        	taps=(1.0 + 1.0j, ),
        	noise_seed=0,
        	block_tags=False
        )
        self.blocks_unpacked_to_packed_xx_0 = blocks.unpacked_to_packed_bb(1, gr.GR_MSB_FIRST)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_repeat_0 = blocks.repeat(gr.sizeof_char*1, samp_sym)
        self.blocks_keep_one_in_n_0 = blocks.keep_one_in_n(gr.sizeof_char*1, 32)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 1000)
        self.Spread_sync_0 = Spread.sync()
        self.Spread_rx_sync_0 = Spread.rx_sync(12)
        self.Spread_preamble_0 = Spread.preamble_bb(1389, (64*(1, 1)))
        self.Spread_msg_source_0 = Spread.msg_source(100, 100)
        self.Spread_framer_0 = Spread.framer(0)
        self.Spread_ds_spreader_0 = Spread.ds_spreader(samp_chip, (generator), (init))
        self.Spread_ds_despreader_0 = Spread.ds_despreader(samp_chip, 
                                     1024, 
                                     2045, 
                                     (generator), 
                                     (init))
        self.Spread_deframer_0 = Spread.deframer(0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_throttle_0, 0), (self.Spread_ds_despreader_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.blocks_repeat_0, 0), (self.Spread_ds_spreader_0, 0))
        self.connect((self.Spread_ds_despreader_0, 0), (self.wxgui_fftsink2_0_0, 0))
        self.connect((self.Spread_sync_0, 0), (self.Spread_preamble_0, 0))
        self.connect((self.Spread_preamble_0, 0), (self.blocks_repeat_0, 0))
        self.connect((self.channels_channel_model_0, 0), (self.blocks_delay_0, 0))
        self.connect((self.blocks_delay_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.Spread_ds_despreader_0, 0), (self.digital_dxpsk_demod_0, 0))
        self.connect((self.digital_dxpsk_demod_0, 0), (self.blocks_keep_one_in_n_0, 0))
        self.connect((self.blocks_keep_one_in_n_0, 0), (self.Spread_rx_sync_0, 0))
        self.connect((self.digital_dxpsk_mod_0, 0), (self.channels_channel_model_0, 0))
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.digital_dxpsk_mod_0, 0))
        self.connect((self.Spread_ds_spreader_0, 0), (self.blocks_unpacked_to_packed_xx_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.Spread_framer_0, "out", self.Spread_sync_0, "in")
        self.msg_connect(self.Spread_msg_source_0, "out", self.Spread_framer_0, "in")
        self.msg_connect(self.Spread_rx_sync_0, "out", self.Spread_deframer_0, "in")


    def get_variable_constellation_0(self):
        return self.variable_constellation_0

    def set_variable_constellation_0(self, variable_constellation_0):
        self.variable_constellation_0 = variable_constellation_0

    def get_samp_sym(self):
        return self.samp_sym

    def set_samp_sym(self, samp_sym):
        self.samp_sym = samp_sym

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0_0.set_sample_rate(self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)

    def get_samp_chip(self):
        return self.samp_chip

    def set_samp_chip(self, samp_chip):
        self.samp_chip = samp_chip

    def get_init(self):
        return self.init

    def set_init(self, init):
        self.init = init

    def get_generator(self):
        return self.generator

    def set_generator(self, generator):
        self.generator = generator

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    (options, args) = parser.parse_args()
    tb = dsss_sync()
    tb.Start(True)
    tb.Wait()
