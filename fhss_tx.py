#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: FHSS transmit
# Author: Paul David
# Generated: Mon Mar  2 14:53:01 2015
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

class fhss_tx(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="FHSS transmit")
        _icon_path = "/usr/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_sym = samp_sym = 64
        self.tx_freq = tx_freq = 2.414e9
        self.samp_rate = samp_rate = 400e3
        self.registers = registers = 3
        self.init = init = 1, 1, 1, 1
        self.generator = generator = 1, 1, 0, 0, 1
        self.code_rate = code_rate = int(samp_sym * 500)

        ##################################################
        # Blocks
        ##################################################
        self._tx_freq_chooser = forms.drop_down(
        	parent=self.GetWin(),
        	value=self.tx_freq,
        	callback=self.set_tx_freq,
        	label="tx_freq",
        	choices=[2414000000, 2432000000, 2450000000, 2468000000],
        	labels=[1,2,3,4],
        )
        self.Add(self._tx_freq_chooser)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("addr=192.168.10.2", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(samp_rate)
        self.uhd_usrp_sink_0.set_center_freq(tx_freq, 0)
        self.uhd_usrp_sink_0.set_gain(20, 0)
        self.uhd_usrp_sink_0.set_antenna("TX/RX", 0)
        self.uhd_usrp_sink_0.set_bandwidth(1.125e6, 0)
        self.digital_gfsk_mod_0 = digital.gfsk_mod(
        	samples_per_symbol=64,
        	sensitivity=1.0,
        	bt=0.35,
        	verbose=False,
        	log=False,
        )
        self.blocks_vector_source_x_0 = blocks.vector_source_b((0, 1, 0, 1), True, 1, [])
        self.blks2_packet_encoder_0 = grc_blks2.packet_mod_b(grc_blks2.packet_encoder(
        		samples_per_symbol=1,
        		bits_per_symbol=1,
        		preamble="",
        		access_code="",
        		pad_for_usrp=True,
        	),
        	payload_length=0,
        )
        self.Spread_synthesizer_0 = Spread.synthesizer(code_rate, 9000, samp_rate, (generator), (init))

        ##################################################
        # Connections
        ##################################################
        self.connect((self.Spread_synthesizer_0, 0), (self.uhd_usrp_sink_0, 0))    
        self.connect((self.blks2_packet_encoder_0, 0), (self.digital_gfsk_mod_0, 0))    
        self.connect((self.blocks_vector_source_x_0, 0), (self.blks2_packet_encoder_0, 0))    
        self.connect((self.digital_gfsk_mod_0, 0), (self.Spread_synthesizer_0, 0))    


    def get_samp_sym(self):
        return self.samp_sym

    def set_samp_sym(self, samp_sym):
        self.samp_sym = samp_sym
        self.set_code_rate(int(self.samp_sym * 500))

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
        self.uhd_usrp_sink_0.set_samp_rate(self.samp_rate)

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
