/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_MIMO_OFDM_INSERT_PREAMBLE_TAGGED_IMPL_H
#define INCLUDED_MIMO_OFDM_INSERT_PREAMBLE_TAGGED_IMPL_H

#include <mimo/ofdm_insert_preamble_tagged.h>
#include <fstream> 

namespace gr {
  namespace mimo {

    class ofdm_insert_preamble_tagged_impl : public ofdm_insert_preamble_tagged
    {
     private:
	int fft_len;
	std::vector< std::vector< gr_complex > > preamble_symbols;
	std::fstream log;

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      ofdm_insert_preamble_tagged_impl(int fft_length, const std::vector< std::vector< gr_complex > > & preamble );
      ~ofdm_insert_preamble_tagged_impl();
	 
	void add_time_tag(const double delay_sec);

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace mimo
} // namespace gr

#endif /* INCLUDED_MIMO_OFDM_INSERT_PREAMBLE_TAGGED_IMPL_H */

