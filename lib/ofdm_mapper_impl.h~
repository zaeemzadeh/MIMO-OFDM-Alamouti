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

#ifndef INCLUDED_MIMO_OFDM_MAPPER_IMPL_H
#define INCLUDED_MIMO_OFDM_MAPPER_IMPL_H

#include <mimo/ofdm_mapper.h>
#include <vector>
#include <fstream> 

namespace gr {
  namespace mimo {

    class ofdm_mapper_impl : public ofdm_mapper
    {
     private:
	static const int HEADERBYTELEN   = 4;
  	static const int HEADERCONSTPOINTS = HEADERBYTELEN*8/2; // The header is qpsk modulated

      	int fft_len;
	int occupied_tones;
	int bits_per_symbol;
	std::vector<gr_complex> constell;
	std::vector<int> subcarrier_map;

	std::fstream log;
	unsigned int make_header(unsigned int bytes_per_packet);

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      ofdm_mapper_impl(const std::vector<gr_complex> &constellation, unsigned int occupied_carriers, unsigned int fft_length);
      ~ofdm_mapper_impl();

      // Where all the action really happens
      int work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace mimo
} // namespace gr

#endif /* INCLUDED_MIMO_OFDM_MAPPER_IMPL_H */

