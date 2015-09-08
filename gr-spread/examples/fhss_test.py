#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: fhss_test
# Author: Paul David
# Generated: Mon Aug 11 18:15:41 2014
##################################################

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import numbersink2
from gnuradio.wxgui import waterfallsink2
from grc_gnuradio import blks2 as grc_blks2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import Spread
import time
import wx

class fhss_test(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="fhss_test")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_sym = samp_sym = 64
        self.samp_rate = samp_rate = 400e3
        self.init = init = 1, 1, 1, 1
        self.generator = generator = 1, 1, 0, 0, 1
        self.code_rate = code_rate = int(samp_sym * 500)

        ##################################################
        # Blocks
        ##################################################
        self.wxgui_waterfallsink2_0 = waterfallsink2.waterfall_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
        	dynamic_range=100,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=512,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="Received Spread Spectrum ",
        )
        self.Add(self.wxgui_waterfallsink2_0.win)
        self.wxgui_numbersink2_0_0_0_0_0 = numbersink2.number_sink_f(
        	self.GetWin(),
        	unit="Units",
        	minval=-100e5,
        	maxval=100e5,
        	factor=1.0,
        	decimal_places=10,
        	ref_level=0,
        	sample_rate=samp_rate,
        	number_rate=60,
        	average=False,
        	avg_alpha=None,
        	label="Error",
        	peak_hold=False,
        	show_gauge=False,
        )
        self.Add(self.wxgui_numbersink2_0_0_0_0_0.win)
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
        	title="Despread Signal",
        	peak_hold=False,
        )
        self.GridAdd(self.wxgui_fftsink2_0.win, 0, 2, 2, 4)
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("", "serial=F54381")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_clock_source("internal", 0)
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0.set_center_freq(500e6, 0)
        self.uhd_usrp_source_0.set_gain(47, 0)
        self.uhd_usrp_source_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("", "serial=F54358")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_clock_source("internal", 0)
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(500e6, 0)
        self.uhd_usrp_sink_0.set_gain(50, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.blocks_vector_source_x_0 = blocks.vector_source_b((1,1,1,1), True, 1, [])
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((.9, ))
        self.blks2_error_rate_0 = grc_blks2.error_rate(
        	type='BER',
        	win_size=1000,
        	bits_per_symbol=1,
        )
        self.Spread_synthesizer_0 = Spread.synthesizer(code_rate, 0, samp_rate, (generator), (init))
        self.Spread_rx_synthesizer_0 = Spread.rx_synthesizer(code_rate, 
                                      samp_sym, 
                                      samp_rate, 
                                      12000, 
                                      50000, 
                                      0.16, 
                                      (generator), 
                                      (init))
        self.Spread_cpfsk_mod_0 = Spread.cpfsk_mod(samp_sym)
        self.Spread_cpfsk_demod_0 = Spread.cpfsk_demod(samp_sym)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blks2_error_rate_0, 0), (self.wxgui_numbersink2_0_0_0_0_0, 0))
        self.connect((self.Spread_cpfsk_demod_0, 0), (self.blks2_error_rate_0, 1))
        self.connect((self.blocks_vector_source_x_0, 0), (self.Spread_cpfsk_mod_0, 0))
        self.connect((self.blocks_vector_source_x_0, 0), (self.blks2_error_rate_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.Spread_rx_synthesizer_0, 0))
        self.connect((self.Spread_synthesizer_0, 0), (self.uhd_usrp_sink_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.wxgui_waterfallsink2_0, 0))
        self.connect((self.Spread_rx_synthesizer_0, 0), (self.Spread_cpfsk_demod_0, 0))
        self.connect((self.Spread_cpfsk_mod_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.Spread_synthesizer_0, 0))
        self.connect((self.Spread_rx_synthesizer_0, 0), (self.wxgui_fftsink2_0, 0))



    def get_samp_sym(self):
        return self.samp_sym

    def set_samp_sym(self, samp_sym):
        self.samp_sym = samp_sym
        self.set_code_rate(int(self.samp_sym * 500))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)
        self.wxgui_waterfallsink2_0.set_sample_rate(self.samp_rate)

    def get_init(self):
        return self.init

    def set_init(self, init):
        self.init = init

    def get_generator(self):
        return self.generator

    def set_generator(self, generator):
        self.generator = generator

    def get_code_rate(self):
        return self.code_rate

    def set_code_rate(self, code_rate):
        self.code_rate = code_rate

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
    tb = fhss_test()
    tb.Start(True)
    tb.Wait()
