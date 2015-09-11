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

#include <gnuradio/io_signature.h>
#include "alamouti_ofdm_tx_cc_impl.h"

namespace gr {
  namespace mimo {

    alamouti_ofdm_tx_cc::sptr
    alamouti_ofdm_tx_cc::make(int fft_length)
    {
      return gnuradio::get_initial_sptr
        (new alamouti_ofdm_tx_cc_impl(fft_length));
    }

    /*
     * The private constructor
     */
    alamouti_ofdm_tx_cc_impl::alamouti_ofdm_tx_cc_impl(int fft_length)
      : gr::block("alamouti_ofdm_tx_cc",
       		gr::io_signature::make(1, 1, sizeof(gr_complex)*200),    //TODO
		gr::io_signature::make(2, 2, sizeof(gr_complex)*fft_length)),
		fft_len(fft_length)
    {
		log.open("log_OFDM_alamouti_encoder.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
	}

    /*
     * Our virtual destructor.
     */
    alamouti_ofdm_tx_cc_impl::~alamouti_ofdm_tx_cc_impl()
    {
	log.close();
    }

    void
    alamouti_ofdm_tx_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = noutput_items;
    }

    int
    alamouti_ofdm_tx_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
   	  const gr_complex *in = (const gr_complex *) input_items[0];
	  gr_complex *out0 = (gr_complex *) output_items[0];
	  gr_complex *out1 = (gr_complex *) output_items[1];

	// initialization 
	memset(out0, 0, noutput_items*fft_len*sizeof(gr_complex));
	memset(out1, 0, noutput_items*fft_len*sizeof(gr_complex));

	int occupied_tones = 200;		// by doing this we can eliminate some calculations
	int unoccupied_carriers = fft_len - occupied_tones;
	int zeros_on_left = (int)ceil(unoccupied_carriers/2.0) ;

	  int i = 0, o = 0; // input and output buffer pointers
	// as long as there is space in output buffer,  symbols are processed
	  while(o < noutput_items) {
		// bad item? discard it!
		if (in[i*occupied_tones + occupied_tones/5] == gr_complex(0,0) || 
			in[i*occupied_tones + 4*occupied_tones/5] == gr_complex(0,0)){
			i++;
			continue;
		}
		// this is where alamouti-coding is taking place
	    	for(int j = 0; j < occupied_tones/2 ; j++) {
	       		out0[o*fft_len + zeros_on_left + 2*j]     = in[i*occupied_tones + 2*j];
			out0[o*fft_len + zeros_on_left + 2*j + 1] = -conj(in[i*occupied_tones + 2*j + 1]);
		}
		for(int j = 0; j < occupied_tones/2 ; j++) {
		       out1[o*fft_len +  zeros_on_left + 2*j] 	 = in[i*occupied_tones + 2*j + 1];
		       out1[o*fft_len +  zeros_on_left + 2*j +1] = conj(in[i*occupied_tones + 2*j]);
		}
		i++;
		o++;
	  }
	
	consume_each(i);	// number of consumed input OFDM-symbols
	return o;		// number of produced output OFDM-symbol (bad items => o != i)
    }

  } /* namespace mimo */
} /* namespace gr */

