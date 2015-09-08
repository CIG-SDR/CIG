#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Thu Feb 26 13:59:27 2015
##################################################

from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from grc_gnuradio import blks2 as grc_blks2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import Spread
import time
import wx

max_time =  int(raw_input("Input Time (sec)"))

class top_block(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Top Block")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 32000

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("addr=192.168.10.2", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(2.415e9, 0)
        self.uhd_usrp_sink_0.set_gain(15, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0.set_bandwidth(8.7e6, 0)
        self.digital_dxpsk_mod_0 = digital.dbpsk_mod(
        	samples_per_symbol=2,
        	excess_bw=0.35,
        	mod_code="gray",
        	verbose=False,
        	log=False)
        	
        self.blocks_vector_source_x_0 = blocks.vector_source_b((1,1,1,1), True, 1, [])
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((.500, ))
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
        self.connect((self.Spread_ds_spreader_0, 0), (self.digital_dxpsk_mod_0, 0))    
        self.connect((self.blks2_packet_encoder_0, 0), (self.Spread_ds_spreader_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.uhd_usrp_sink_0, 0))    
        self.connect((self.blocks_vector_source_x_0, 0), (self.blks2_packet_encoder_0, 0))    
        self.connect((self.digital_dxpsk_mod_0, 0), (self.blocks_multiply_const_vxx_0, 0))    


    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)

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
    time.sleep(max_time)
    #tb.show()
    quit()
   # tb.Wait()
