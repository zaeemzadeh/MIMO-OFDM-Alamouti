#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import mimo_swig as mimo

class qa_alamouti_decode_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def notest_001_t (self):
	expected_result = (-3 + 1j , 4 + 2j, -5 + 5j, 2)
	#alamouti code ant1
        src_data0 = (-3 + 1j, -4 + 2j, -5 + 5j, -2)
	#alamouti code ant2
	src_data1 = (4 + 2j, -3 - 1j , 2 , -5 -5j)
	#simple channel
	input_data= (1 + 3j , -7 + 1j , -3 + 5j , -7 - 5j) #src_data0 + src_data1

        src = blocks.vector_source_c(input_data)
        mm = mimo.alamouti_decode_cc()
        dst = blocks.vector_sink_c()
        self.tb.connect(src, (mm,0))
	self.tb.connect(src, (mm,1))
	self.tb.connect(mm, dst)
        self.tb.run ()
        # check data
	result_data = dst.data()
        self.assertComplexTuplesAlmostEqual(expected_result, result_data, 5)


if __name__ == '__main__':
    gr_unittest.run(qa_alamouti_decode_cc, "qa_alamouti_decode_cc.xml")
