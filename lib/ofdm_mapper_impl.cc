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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cstdio>
//#include <math>
#include <gnuradio/io_signature.h>
#include "ofdm_mapper_impl.h"

namespace gr {
  namespace mimo {

	ofdm_mapper::sptr
	ofdm_mapper::make(const std::vector<gr_complex> &constellation, unsigned int occupied_carriers, unsigned int fft_length)
	{
	  return gnuradio::get_initial_sptr
		(new ofdm_mapper_impl(constellation, occupied_carriers, fft_length));
	}

	/*
	 * The private constructor
	 */
	ofdm_mapper_impl::ofdm_mapper_impl(const std::vector<gr_complex> &constellation, unsigned int occupied_carriers, unsigned int fft_length)
	  : gr::tagged_stream_block("ofdm_mapper",
			  gr::io_signature::make(1,1, sizeof(char)),
			  gr::io_signature::make2(2, 2, sizeof(gr_complex)*occupied_carriers, sizeof(char)),
		"packet_len"), 
	fft_len(fft_length),
	occupied_tones(occupied_carriers) //TODO fft
	{
		log.open("log_OFDM_mapper.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

		set_tag_propagation_policy(TPP_DONT); 	// do not propagate tags

		//QPSK
		constell.push_back(gr_complex(1,1));
		constell.push_back(gr_complex(-1,1));
		constell.push_back(gr_complex(-1,-1));
		constell.push_back(gr_complex(1,-1));
		bits_per_symbol = 2;

		//subcarrier map
		// 4 subcarriers in center (DC) are not used
		for (int i = occupied_tones/2 ; i > 2 ; i--){
			subcarrier_map.push_back(occupied_tones/2 - i);
		}

		for (int i = 2 ; i < occupied_tones/2 ; i++){
			subcarrier_map.push_back(occupied_tones/2 + i );
		}		
	}

	/*
	 * Our virtual destructor.
	 */
	ofdm_mapper_impl::~ofdm_mapper_impl()
	{
	log.close();
	}

	int
	ofdm_mapper_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
	{
		// here we should calculate how many ofdm symbols are produced
		// by each packet
		int bytes_per_packet = ninput_items[0] + HEADERBYTELEN;
	  	float noutput_items = (bytes_per_packet*8.0)/(bits_per_symbol*(occupied_tones-4)); 
	  	return int(ceil(noutput_items)) ;
	}
   	unsigned int
	ofdm_mapper_impl::make_header(unsigned int bytes_per_packet){
  	// header format upper nibble is unused the three lower nibble contains nr of bytes in one packet.
  	// the 16 bit header is copied so that the receiver can check if the header is correct by comparing
  	// both parts.
  		return ((bytes_per_packet) & 0x0fff) | (((bytes_per_packet) & 0x0fff) << 16);
 	 }

	int
	ofdm_mapper_impl::work (int noutput_items,
					   gr_vector_int &ninput_items,
					   gr_vector_const_void_star &input_items,
					   gr_vector_void_star &output_items)
	{
		const char *in = (const char *) input_items[0];
		gr_complex *out = (gr_complex *) output_items[0];
		char *flag = (char *) output_items[1];
		//each OFDM symbol is implemented as a vector of complex symbols
		// with length of fft length

		float symbol_per_packet;		//OFDM symbols per input packet
		int bytes_per_packet = ninput_items[0] + HEADERBYTELEN;
		symbol_per_packet = (bytes_per_packet*8.0)/(bits_per_symbol*(occupied_tones-4.0));
		symbol_per_packet = ceil(symbol_per_packet);

		// we set memory to zero (initialization)
		memset(out, 0, symbol_per_packet*occupied_tones*sizeof(gr_complex));//TODO
		memset(flag, 0, symbol_per_packet*sizeof(char));
		
		int ton = 0;		//current subcarrier
		int symbol = 0;		//OFDM symbol
		unsigned char chunk ;
		flag[0] = 1; // marks begining of payload in output
		// here we add length tag to output stream
		const uint64_t nwritten = nitems_written(0);
		const pmt::pmt_t key   = pmt::string_to_symbol(std::string("packet_len"));
		const pmt::pmt_t value = pmt::from_long(long(symbol_per_packet));
		add_item_tag(0, nwritten,key,value);

		// for each input packet, header is created and transmitted
		unsigned int header_tmp = make_header( ninput_items[0]);
		for (ton = 0; ton < HEADERCONSTPOINTS; ton++ ) {
			int bit_shift = (HEADERCONSTPOINTS - 1 - ton)*2;
			chunk = (unsigned char) ((header_tmp >> bit_shift) & 0x03);
			out[symbol*occupied_tones + subcarrier_map[ton] -subcarrier_map[0]]=  constell[chunk];
		}
	
		// after header, data will be transmitted
		int consumed_bits = 0;
		for (symbol = 0; symbol< symbol_per_packet; symbol++){
			for (; ton < subcarrier_map.size() ; ton++)
			{
				// extracting chunks
				chunk = in[consumed_bits/8];
				int resid_bits = consumed_bits%8;
				chunk = chunk & (192 >> resid_bits);  // 192 = 0b11000000
				chunk = (chunk >> (6-resid_bits));
		
				out[symbol*occupied_tones + subcarrier_map[ton] -subcarrier_map[0] ] 
						= constell[chunk];	
				consumed_bits += bits_per_symbol;
			}
			ton = 0;
		}
		// Tell runtime system new packet length.
		return symbol_per_packet; 
	}

  } /* namespace mimo */
} /* namespace gr */

