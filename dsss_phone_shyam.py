#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon Mar  2 14:36:30 2015
##################################################

from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.wxgui import forms
from grc_gnuradio import blks2 as grc_blks2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import Spread
import time
import wx

class top_block(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Top Block")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.tx_freq = tx_freq = 2.407e9
        self.samp_rate = samp_rate = 32e3

        ##################################################
        # Blocks
        ##################################################
        self._tx_freq_chooser = forms.drop_down(
        	parent=self.GetWin(),
        	value=self.tx_freq,
        	callback=self.set_tx_freq,
        	label="tx_freq",
        	choices=[2.407e9, 2.41e9, 2.413e9, 2.416e9, 2.419e9, 2.422e9, 2.425e9, 2.427e9, 2.429e9, 2.431e9, 2.433e9, 2.436e9, 2.439e9, 2.441e9, 2.443e9,2.422e9, 2.446e9, 2.449e9, 2.452e9, 2.455e9,  2.458e9, 2.460e9, 2.463e9, 2.466e9, 2.469e9, 2.472e9, 2.475e9, 2.478e9 ],
        	labels=[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28],
        )
        self.Add(self._tx_freq_chooser)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("addr=192.168.10.2", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(32e3)
        self.uhd_usrp_sink_0.set_center_freq(tx_freq, 0)
        self.uhd_usrp_sink_0.set_gain(20, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0.set_bandwidth(3e6, 0)
        self.digital_gfsk_mod_0 = digital.gfsk_mod(
        	samples_per_symbol=2,
        	sensitivity=1.0,
        	bt=0.35,
        	verbose=False,
        	log=False,
        )
        self.blocks_vector_source_x_0 = blocks.vector_source_b((1,0,1,0), True, 1, [])
        self.blks2_packet_encoder_0 = grc_blks2.packet_mod_b(grc_blks2.packet_encoder(
        		samples_per_symbol=1,
        		bits_per_symbol=1,
        		preamble="",
        		access_code="",
        		pad_for_usrp=True,
        	),
        	payload_length=0,
        )
        self.Spread_ds_spreader_0 = Spread.ds_spreader(2, (1, 1, 0, 1), (1, 1, 1))

        ##################################################
        # Connections
        ##################################################
        self.connect((self.Spread_ds_spreader_0, 0), (self.digital_gfsk_mod_0, 0))    
        self.connect((self.blks2_packet_encoder_0, 0), (self.Spread_ds_spreader_0, 0))    
        self.connect((self.blocks_vector_source_x_0, 0), (self.blks2_packet_encoder_0, 0))    
        self.connect((self.digital_gfsk_mod_0, 0), (self.uhd_usrp_sink_0, 0))    


    def get_tx_freq(self):
        return self.tx_freq

    def set_tx_freq(self, tx_freq):
        self.tx_freq = tx_freq
        self.uhd_usrp_sink_0.set_center_freq(self.tx_freq, 0)
        self._tx_freq_chooser.set_value(self.tx_freq)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate

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
    tb = top_block()
    tb.Start(True)
    tb.Wait()
