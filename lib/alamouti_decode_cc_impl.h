/* -*- c++ -*- */
/* 
 * Copyright 2014  Alireza Zaeemzadeh.
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

#ifndef INCLUDED_MIMO_ALAMOUTI_DECODE_CC_IMPL_H
#define INCLUDED_MIMO_ALAMOUTI_DECODE_CC_IMPL_H


#include <mimo/alamouti_decode_cc.h>


namespace gr {
  namespace mimo {

    class alamouti_decode_cc_impl : public alamouti_decode_cc
    {
     private:
      	int payload_length;		// number of payload symbols between two consequent pilot 
	int pilot_length;		// number of pilot symbols transmited by each antenna

	gr_complex pilot_symbol[4];	// pilot symbols

	int symbols_recieved;		
	int pilots_recieved;
	int state;
	//Channel State Information
	gr_complex h00;
	gr_complex h01;
	gr_complex h10;
	gr_complex h11;
	gr_complex norm;

	// receivrd pilots
	gr_complex rcvd_pilot_rx0;
	gr_complex rcvd_pilot_rx1;
	std::fstream log;
	void recieve_pilot (int &consumed_inputs,const int noutput_items,
			const gr_complex *in0, const gr_complex *in1) ;

	

     public:
      alamouti_decode_cc_impl();
      ~alamouti_decode_cc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace mimo
} // namespace gr

#endif /* INCLUDED_MIMO_ALAMOUTI_DECODE_CC_IMPL_H */

