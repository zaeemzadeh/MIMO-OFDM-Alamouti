/* -*- c++ -*- */
/* 
 * Copyright 2014 Alireza Zaeemzadeh.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_MIMO_ALAMOUTI_ENCODE_CC_IMPL_H
#define INCLUDED_MIMO_ALAMOUTI_ENCODE_CC_IMPL_H

#include <mimo/alamouti_encode_cc.h>

namespace gr {
  namespace mimo {

    class alamouti_encode_cc_impl : public alamouti_encode_cc
    {
     private:
	int payload_length;		// number of payload symbols between two consequent pilot 
	int pilot_length;		// number of pilot symbols transmited by each antenna
	int num_of_packs; 		// number of packets between consequent preambles
	int preamble_length;		// number of preamble symbols

	gr_complex pilot_symbol[4];	// pilot symbols
	int preamble[64];		// preamble sequence (GOLD)

	// following parameters indicte the state of the transmitter {PILOT,PREAMBLE,PAYLOAD}
	int packets_sent;		
	int symbols_sent;
	int pilots_sent;
	int preambles_sent;
	int state;			
	std::fstream log;	// discriptor of log file
	

	void send_pilot(int &generated_outputs, const int noutput_items,
				gr_complex *out0,  gr_complex *out1);
	void send_preamble(int &generated_outputs, const int noutput_items,
				gr_complex *out0,  gr_complex *out1);

     public:
      alamouti_encode_cc_impl();
      ~alamouti_encode_cc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace mimo
} // namespace gr

#endif /* INCLUDED_MIMO_ALAMOUTI_ENCODE_CC_IMPL_H */

