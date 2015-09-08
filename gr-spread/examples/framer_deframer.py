#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Frame / sync / deframer chain
# Author: Paul David
# Generated: Tue Sep 16 17:25:00 2014
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

class framer_deframer(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Frame / sync / deframer chain")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 500e3

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
        self.digital_gmsk_mod_0 = digital.gmsk_mod(
        	samples_per_symbol=32,
        	bt=0.35,
        	verbose=False,
        	log=False,
        )
        self.digital_gmsk_demod_0 = digital.gmsk_demod(
        	samples_per_symbol=32,
        	gain_mu=0.175,
        	mu=0.5,
        	omega_relative_limit=0.01,
        	freq_error=0.0,
        	verbose=False,
        	log=False,
        )
        self.channels_channel_model_0 = channels.channel_model(
        	noise_voltage=0.001,
        	frequency_offset=0.0,
        	epsilon=1.000001,
        	taps=(1.0 + 1.0j, ),
        	noise_seed=0,
        	block_tags=False
        )
        self.blocks_unpacked_to_packed_xx_0 = blocks.unpacked_to_packed_bb(1, gr.GR_MSB_FIRST)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((20, ))
        self.Spread_sync_0 = Spread.sync()
        self.Spread_rx_sync_0 = Spread.rx_sync(12)
        self.Spread_msg_source_0 = Spread.msg_source(500, 255)
        self.Spread_framer_0 = Spread.framer(0)
        self.Spread_deframer_0 = Spread.deframer(0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.Spread_sync_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.blocks_unpacked_to_packed_xx_0, 0))
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.digital_gmsk_mod_0, 0))
        self.connect((self.digital_gmsk_mod_0, 0), (self.channels_channel_model_0, 0))
        self.connect((self.digital_gmsk_demod_0, 0), (self.Spread_rx_sync_0, 0))
        self.connect((self.channels_channel_model_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.channels_channel_model_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.digital_gmsk_demod_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.Spread_framer_0, "out", self.Spread_sync_0, "in")
        self.msg_connect(self.Spread_msg_source_0, "out", self.Spread_framer_0, "in")
        self.msg_connect(self.Spread_rx_sync_0, "out", self.Spread_deframer_0, "in")


    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)

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
    tb = framer_deframer()
    tb.Start(True)
    tb.Wait()
