#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: FHSS transmit
# Author: Paul David
# Generated: Wed Feb 18 10:53:05 2015
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
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import Spread
import time
import wx

class fhss_tx(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="FHSS transmit")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_sym = samp_sym = 64
        self.samp_rate = samp_rate = 400e3
        self.registers = registers = 3
        self.init = init = 1, 1, 1, 1
        self.generator = generator = 1, 1, 0, 0, 1
        self.code_rate = code_rate = int(samp_sym * 500)

        ##################################################
        # Blocks
        ##################################################
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
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.Add(self.wxgui_fftsink2_0.win)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("dev_addr=192.168.10.2", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(2.45e9, 0)
        self.uhd_usrp_sink_0.set_gain(30, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.blocks_vector_source_x_0 = blocks.vector_source_b((0, 1, 0, 1), True, 1, [])
        self.Spread_synthesizer_0 = Spread.synthesizer(code_rate, 9000, samp_rate, (generator), (init))
        self.Spread_cpfsk_mod_0 = Spread.cpfsk_mod(samp_sym)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.Spread_cpfsk_mod_0, 0), (self.Spread_synthesizer_0, 0))    
        self.connect((self.Spread_synthesizer_0, 0), (self.uhd_usrp_sink_0, 0))    
        self.connect((self.Spread_synthesizer_0, 0), (self.wxgui_fftsink2_0, 0))    
        self.connect((self.blocks_vector_source_x_0, 0), (self.Spread_cpfsk_mod_0, 0))    


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
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)

    def get_registers(self):
        return self.registers

    def set_registers(self, registers):
        self.registers = registers

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
    tb = fhss_tx()
    tb.Start(True)
    tb.Wait()
