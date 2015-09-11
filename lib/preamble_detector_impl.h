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

#ifndef INCLUDED_MIMO_PREAMBLE_DETECTOR_IMPL_H
#define INCLUDED_MIMO_PREAMBLE_DETECTOR_IMPL_H

#include <mimo/preamble_detector.h>

namespace gr {
  namespace mimo {

    class preamble_detector_impl : public preamble_detector
    {
     private:
	//block parameters
	int preamble_length; 	//in symbols
	int packet_length; 	//in symbols : pilots + payloads
	int num_of_packets;	//between consequent preambles
	int samp_per_symb;	//samples per symbol

	bool preamble_mode;	// state of system
	int samples_sent;
	std::fstream log;
	int transient_counter;	// counts the transient samples (these samples are discarde)
	int preambles_received;
	int history;		// history of system [in samples]

     public:
      preamble_detector_impl();
      ~preamble_detector_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace mimo
} // namespace gr

#endif /* INCLUDED_MIMO_PREAMBLE_DETECTOR_IMPL_H */

