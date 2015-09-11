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

class qa_alamouti_encode_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
	src_data = (-3 + 1j , 4 + 2j, -5 + 5j, 2)
	for i in range(6000):
		src_data = src_data + (-3 + 1j , 4 + 2j, -5 + 5j, 2)
        #expected_result0 = (-3 + 1j, -4 + 2j, -5 + 5j, -2)
	#expected_result1 = (4 + 2j, -3 - 1j , 2 , -5 -5j)
        src = blocks.vector_source_c(src_data)
        mmtx = mimo.alamouti_encode_cc()

	add = blocks.add_cc()

	mmrx = mimo.alamouti_decode_cc()
        dst = blocks.vector_sink_c()

        self.tb.connect(src, mmtx)
        self.tb.connect((mmtx,0), (add,0))
	self.tb.connect((mmtx,1), (add,1))
	self.tb.connect(add, (mmrx,0))
	self.tb.connect(add, (mmrx,1))
	self.tb.connect(mmrx, dst)
        self.tb.run ()
        # check data
	result_data = dst.data()
	print len(result_data)
	#for i in range(len(result_data)):
	#	print result_data[i]
	#	print src_data[i]
        self.assertComplexTuplesAlmostEqual(src_data , result_data, 1025)
	#self.assertComplexTuplesAlmostEqual(expected_result1, result_data1, 5)

    def notest_001_t (self):
	src_data = (-3 + 1j , 4 + 2j, -5 + 5j, 2)
	#alamouti code ant1
        exp_data0 = (-3 + 1j, -4 + 2j, -5 + 5j, -2)
	#alamouti code ant2
	exo_data1 = (4 + 2j, -3 - 1j , 2 , -5 -5j)

        src = blocks.vector_source_c(src_data)
        mm = mimo.alamouti_encode_cc()
        dst0 = blocks.vector_sink_c()	
        dst1 = blocks.vector_sink_c()
        self.tb.connect(src, mm)
	self.tb.connect((mm,0), dst0)
	self.tb.connect((mm,1), dst1)
        self.tb.run ()
        # check data
	result_data0 = dst0.data()	
	result_data1 = dst1.data()
        self.assertComplexTuplesAlmostEqual(exp_data0, result_data0, 5)
        self.assertComplexTuplesAlmostEqual(exp_data1, result_data1, 5)


if __name__ == '__main__':
    gr_unittest.run(qa_alamouti_encode_cc, "qa_alamouti_encode_cc.xml")
