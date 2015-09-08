#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Fhss Sync
# Author: Paul David
# Generated: Tue Sep 16 17:24:30 2014
##################################################

from gnuradio import analog
from gnuradio import blocks
from gnuradio import channels
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import forms
from gnuradio.wxgui import numbersink2
from gnuradio.wxgui import waterfallsink2
from grc_gnuradio import blks2 as grc_blks2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import Spread
import numpy
import wx

class fhss_sync(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Fhss Sync")

        ##################################################
        # Variables
        ##################################################
        self.samp_sym = samp_sym = 64
        self.tone_freq = tone_freq = 2000
        self.samp_rate = samp_rate = 800e3
        self.init = init = 1, 1, 1, 1
        self.generator = generator = 1, 1, 0, 0, 1
        self.code_rate = code_rate = int(samp_sym * 10000)

        ##################################################
        # Blocks
        ##################################################
        _tone_freq_sizer = wx.BoxSizer(wx.VERTICAL)
        self._tone_freq_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_tone_freq_sizer,
        	value=self.tone_freq,
        	callback=self.set_tone_freq,
        	label="Tone Frequency",
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._tone_freq_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_tone_freq_sizer,
        	value=self.tone_freq,
        	callback=self.set_tone_freq,
        	minimum=-1 * samp_rate / 2,
        	maximum=samp_rate / 2,
        	num_steps=100,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_tone_freq_sizer)
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
        self.wxgui_numbersink2_0 = numbersink2.number_sink_f(
        	self.GetWin(),
        	unit="Units",
        	minval=0,
        	maxval=1,
        	factor=1.0,
        	decimal_places=10,
        	ref_level=0,
        	sample_rate=samp_rate,
        	number_rate=15,
        	average=False,
        	avg_alpha=None,
        	label="Error Rate",
        	peak_hold=False,
        	show_gauge=False,
        )
        self.Add(self.wxgui_numbersink2_0.win)
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
        self.channels_channel_model_0 = channels.channel_model(
        	noise_voltage=0.1,
        	frequency_offset=1 / samp_rate,
        	epsilon=1 + 0.0000,
        	taps=(1.0 + 1.0j, ),
        	noise_seed=0,
        	block_tags=False
        )
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.blks2_error_rate_0 = grc_blks2.error_rate(
        	type='BER',
        	win_size=1000,
        	bits_per_symbol=2,
        )
        self.analog_sig_source_x_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, tone_freq, 2, 0)
        self.analog_random_source_x_0 = blocks.vector_source_b(map(int, numpy.random.randint(0, 2, 1000)), True)
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
        self.connect((self.Spread_rx_synthesizer_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.Spread_rx_synthesizer_0, 0), (self.blocks_throttle_0, 0))
        self.connect((self.blocks_throttle_0, 0), (self.Spread_cpfsk_demod_0, 0))
        self.connect((self.Spread_cpfsk_mod_0, 0), (self.Spread_synthesizer_0, 0))
        self.connect((self.analog_random_source_x_0, 0), (self.blks2_error_rate_0, 0))
        self.connect((self.analog_random_source_x_0, 0), (self.Spread_cpfsk_mod_0, 0))
        self.connect((self.Spread_cpfsk_demod_0, 0), (self.blks2_error_rate_0, 1))
        self.connect((self.blks2_error_rate_0, 0), (self.wxgui_numbersink2_0, 0))
        self.connect((self.blocks_add_xx_0, 0), (self.channels_channel_model_0, 0))
        self.connect((self.Spread_synthesizer_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.channels_channel_model_0, 0), (self.wxgui_waterfallsink2_0, 0))
        self.connect((self.channels_channel_model_0, 0), (self.Spread_rx_synthesizer_0, 0))



    def get_samp_sym(self):
        return self.samp_sym

    def set_samp_sym(self, samp_sym):
        self.samp_sym = samp_sym
        self.set_code_rate(int(self.samp_sym * 10000))

    def get_tone_freq(self):
        return self.tone_freq

    def set_tone_freq(self, tone_freq):
        self.tone_freq = tone_freq
        self._tone_freq_slider.set_value(self.tone_freq)
        self._tone_freq_text_box.set_value(self.tone_freq)
        self.analog_sig_source_x_0.set_frequency(self.tone_freq)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.channels_channel_model_0.set_frequency_offset(1 / self.samp_rate)
        self.wxgui_waterfallsink2_0.set_sample_rate(self.samp_rate)
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)

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
    tb = fhss_sync()
    tb.Start(True)
    tb.Wait()
