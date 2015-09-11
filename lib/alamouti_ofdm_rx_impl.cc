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
#include "alamouti_ofdm_rx_impl.h"
#include <gnuradio/expj.h>
#include <gnuradio/math.h>
#include <stdexcept>
#include <iostream>
#include <string.h>
#include <cstdio>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#define M_TWOPI (2*M_PI)	//2*pi
#define MAX_NUM_SYMBOLS 1000
#define VERBOSE 0


namespace gr {
  namespace mimo {

    alamouti_ofdm_rx::sptr
    alamouti_ofdm_rx::make(unsigned int occupied_carriers, unsigned int fft_length, unsigned int cplen)
    {
      return gnuradio::get_initial_sptr
        (new alamouti_ofdm_rx_impl(occupied_carriers, fft_length, cplen));
    }

    /*
     * The private constructor
     */
    alamouti_ofdm_rx_impl::alamouti_ofdm_rx_impl(unsigned int occupied_carriers, unsigned int fft_length, unsigned int cplen)
      : gr::block("alamouti_ofdm_rx",
              gr::io_signature::make2(3, 3, sizeof(char)*fft_length, sizeof(gr_complex)*fft_length),
              gr::io_signature::make2(2, 2, sizeof(char), sizeof(gr_complex)*occupied_carriers)),
   d_occupied_carriers(occupied_carriers),
   d_fft_length(fft_length),
   d_cplen(cplen),
   freq_shift_len(10),
   phase_count(0)
    {
	log.open("log_OFDM_decoder.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
	//known preamble #1
	int temp1 [] = {-1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0};

	for (int i = 0; i < d_occupied_carriers; i++) {
		known_symbol0.push_back(temp1[i]);
	}
	//known preamble #2
	int temp2 [] = {1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0};

	for (int i = 0; i < d_occupied_carriers; i++) {
		known_symbol1.push_back(temp2[i]);
	}

 	int ntxchannels = 2;
	int nrxchannels = 2;

	// memory allocation
   	coarse_freq1 = new int[ntxchannels];
   	coarse_freq2 = new int[ntxchannels];
	for (int i = 0 ; i < ntxchannels ; i++){
		coarse_freq1[i] = 0;
		coarse_freq2[i] = 0;
	}


   	hestimate1 = new std::vector<gr_complex>[ntxchannels];
   	hestimate2 = new std::vector<gr_complex>[ntxchannels];
 

	symbol_phase_diff1.resize(d_fft_length);
	symbol_phase_diff2.resize(d_fft_length);

	known_phase_diff1.resize(d_occupied_carriers);
	known_phase_diff2.resize(d_occupied_carriers);

 

   	for(int i=0; i < ntxchannels; i++) {
     		hestimate1[i].resize(d_occupied_carriers);
     		hestimate2[i].resize(d_occupied_carriers);
  	}

   	for(int i=0; i < ntxchannels; i++) {
     		for(unsigned int k=0; k<d_occupied_carriers; k++) {
     			hestimate1[i][k] = gr_complex(1.0,0.0);  
     			hestimate2[i][k] = gr_complex(1.0,0.0);
     		}
   	}

 	std::fill(known_phase_diff1.begin(), known_phase_diff1.end(), 0);
	for(int i = 0; i < known_symbol0.size()-1; i++) {
     		known_phase_diff1[i] = norm(known_symbol0[i] - known_symbol0[i+1]);
	}
 
   	std::fill(known_phase_diff2.begin(), known_phase_diff2.end(), 0);
   	for(int i = 0; i < known_symbol1.size()-1; i++) {
     		known_phase_diff2[i] = norm(known_symbol1[i] - known_symbol1[i+1]);
   	}

		//initial state of reciever:
		first_preamble = false;
		second_preamble = false;
	}
    /*
     * Our virtual destructor.
     */
    alamouti_ofdm_rx_impl::~alamouti_ofdm_rx_impl()
    {
	delete [] hestimate1;
	delete [] hestimate2;
    }

    void
    alamouti_ofdm_rx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
	// here we process ofdm symbols one by one
	unsigned ninputs = ninput_items_required.size ();
   	for (unsigned i = 0; i < ninputs; i++)
     		ninput_items_required[i] = 1;
    
    }

    gr_complex
    alamouti_ofdm_rx_impl::coarse_freq_comp(int freq_delta, int symbol_count)
    {
   	return gr_expj(-M_TWOPI*freq_delta*d_cplen/d_fft_length*symbol_count);
    }

	// coarse frequency offset estimation, by correlating symbol phase diffs
    void
    alamouti_ofdm_rx_impl::correlate1(const gr_complex *symbol0, const gr_complex *symbol1, int zeros_on_left)
    {
	   unsigned int i,j;

	   std::fill(symbol_phase_diff1.begin(), symbol_phase_diff1.end(), 0);
	   std::fill(symbol_phase_diff2.begin(), symbol_phase_diff2.end(), 0);

	   // calcuating recieved preamble diff
	   for(i = 0; i < d_fft_length-1; i++) {
     		symbol_phase_diff1[i] = norm(symbol0[i] - symbol0[i+1]);
     		symbol_phase_diff2[i] = norm(symbol1[i] - symbol1[i+1]);
	    }

	   // sweep through all possible/allowed frequency offsets and select the best
	   int index1 = 0;
	   float max1 = 0, sum1=0;
	   int index2 = 0;
	   float max2 = 0, sum2=0;
	   for(i =  zeros_on_left - freq_shift_len; i < zeros_on_left + freq_shift_len; i++) {
	     sum1 = 0;
	     sum2 = 0;
	     
		// inner product
	     for(j = 0; j < d_occupied_carriers; j++) {
	       sum1 += (known_phase_diff1[j] * symbol_phase_diff1[i+j]);
	       sum2 += (known_phase_diff1[j] * symbol_phase_diff2[i+j]);
	     }
	     
	     if(sum1 > max1) {
	       max1 = sum1;
	       index1 = i;
	     }
	     if(sum2 > max2) {
	       max2 = sum2;
	       index2 = i;
	     }
	   }
	   // set the coarse frequency offset relative to the edge of the occupied tones
	   coarse_freq1[0] = index1 - zeros_on_left;
	   coarse_freq2[0] = index2 - zeros_on_left;
	if (VERBOSE)
		printf("coarse freq[0][0]: %d coarse freq[0][1]: %d \n", coarse_freq1[0], coarse_freq2[0]);
   }

   void
   alamouti_ofdm_rx_impl::correlate2(const gr_complex *symbol0, const gr_complex *symbol1, int zeros_on_left)
   {
  	unsigned int i,j;

	  std::fill(symbol_phase_diff1.begin(), symbol_phase_diff1.end(), 0);
	  std::fill(symbol_phase_diff2.begin(), symbol_phase_diff2.end(), 0);
	// calcuating received preamble diff
	  for(i = 0; i < d_fft_length-1; i++) {
		symbol_phase_diff1[i] = norm(symbol0[i] - symbol0[i+1]);
		symbol_phase_diff2[i] = norm(symbol1[i] - symbol1[i+1]);
	  }

	  // sweep through all possible/allowed frequency offsets and select the best
	  int index1 = 0;
	  float max1 = 0, sum1=0;
	  int index2 = 0;
	  float max2 = 0, sum2=0;
	  for(i =  zeros_on_left - freq_shift_len; i < zeros_on_left + freq_shift_len; i++) {
		    sum1 = 0;
		    sum2 = 0;
		// inner product
		    for(j = 0; j < d_occupied_carriers; j++) {
		      sum1 += (known_phase_diff2[j] * symbol_phase_diff1[i+j]);
		      sum2 += (known_phase_diff2[j] * symbol_phase_diff2[i+j]);
		    }
		    
		    if(sum1 > max1) {
		      max1 = sum1;
		      index1 = i;
		    }
		    if(sum2 > max2) {
		      max2 = sum2;
		      index2 = i;
		    }
	  }
	  // set the coarse frequency offset relative to the edge of the occupied tones
	  coarse_freq1[1] = index1 - zeros_on_left;
	  coarse_freq2[1] = index2 - zeros_on_left;
	if (VERBOSE)
		printf("coarse freq[1][0]: %d coarse freq[1][1]: %d \n", coarse_freq1[1], coarse_freq2[1]);
   }


   void
   alamouti_ofdm_rx_impl::calculate_equalizer(int channel, const gr_complex *symbol1, const gr_complex *symbol2, int zeros_on_left)
   {
	   unsigned int i=0;

	   std::vector<gr_complex> preamble;
	   if(channel == 0){
		preamble = known_symbol0;
		     // Set first tap of equalizer
		hestimate1[channel][0] 
			= (coarse_freq_comp(coarse_freq1[0],1)*symbol1[zeros_on_left+coarse_freq1[0]])/preamble[0];
	     	hestimate2[channel][0] 
			= (coarse_freq_comp(coarse_freq2[0],1)*symbol2[zeros_on_left+coarse_freq2[0]])/preamble[0];
	   
	     for(i = 1; i < d_occupied_carriers; i++) {
	     	hestimate1[channel][i] 
		    =(coarse_freq_comp(coarse_freq1[0],1)*(symbol1[i+zeros_on_left+coarse_freq1[0]]))/preamble[i];
	       hestimate2[channel][i] 
		    = (coarse_freq_comp(coarse_freq2[0],1)*(symbol2[i+zeros_on_left+coarse_freq2[0]]))/preamble[i];
	     }

	     // with even number of carriers; last equalizer tap is wrong
	     if(!(d_occupied_carriers & 1)) {
	       hestimate1[channel][d_occupied_carriers-1] = hestimate1[channel][d_occupied_carriers-2];
	       hestimate2[channel][d_occupied_carriers-1] = hestimate2[channel][d_occupied_carriers-2];
	     }
	   } 
	   else {
	   	preamble = known_symbol1;
	  
	     // Set first tap of equalizer
	   	hestimate1[channel][0] 
		   = (coarse_freq_comp(coarse_freq1[1],2)*symbol1[zeros_on_left+coarse_freq1[1]])/preamble[0];
	       hestimate2[channel][0]
		   = (coarse_freq_comp(coarse_freq2[1],2)*symbol2[zeros_on_left+coarse_freq2[1]])/preamble[0];

	     // set every even tap based on known symbol
	     // linearly interpolate between set carriers to set zero-filled carriers
	     for(i = 1; i < d_occupied_carriers; i++) {
		hestimate1[channel][i] 
		    = (coarse_freq_comp(coarse_freq1[1],2)*(symbol1[i+zeros_on_left+coarse_freq1[1]]))/preamble[i];
	        hestimate2[channel][i] 
		    = (coarse_freq_comp(coarse_freq2[1],2)*(symbol2[i+zeros_on_left+coarse_freq2[1]]))/preamble[i];
	     }

	   // with even number of carriers; last equalizer tap is wrong
	     if(!(d_occupied_carriers & 1)) {
	       hestimate1[channel][d_occupied_carriers-1] = hestimate1[channel][d_occupied_carriers-2];
	       hestimate2[channel][d_occupied_carriers-1] = hestimate2[channel][d_occupied_carriers-2];
	     }
	   }
		if (VERBOSE){
			log << "Channel estimation for TX " << channel << std::endl;
			log << " RX 0 \n";
			for (int i = 0; i < d_occupied_carriers ; i++){
				log << hestimate1[channel][i] << std::endl;
			}
			log << " RX 1 \n";
			for (int i = 0; i < d_occupied_carriers ; i++){
				log << hestimate2[channel][i] << std::endl;
			}
		}
   }


    int
    alamouti_ofdm_rx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const char *signal_in = (const char *)input_items[0];
	   const gr_complex *symbol1 = (const gr_complex *)input_items[1];
	   const gr_complex *symbol2 = (const gr_complex *)input_items[2];

	 
	   char *signal_out = (char *) output_items[0]; 
	   gr_complex *out = (gr_complex *) output_items[1];
	 
	   int unoccupied_carriers = d_fft_length - d_occupied_carriers;
	   int zeros_on_left = (int)ceil(unoccupied_carriers/2.0);

	   signal_out[0] = 0;
	   // Test if we hit the start of a preamble
	   //signal_in  from sampler block
	   if(signal_in[0]) {
	     	first_preamble = true;
	   }
	   else if(first_preamble){
	     	first_preamble = false;
	     	phase_count = 1;

	     	correlate1(&symbol1[0], &symbol2[0], zeros_on_left);
	     
	    	 calculate_equalizer(0, &symbol1[0], &symbol2[0], zeros_on_left);
	   
	    	 signal_out[0] = 1; //signal to demapper block
	    	 second_preamble = true;

	     // equalize the 1st preamble
	        for(unsigned int i = 0; i < d_occupied_carriers; i+=2) {
			 out[i] = (coarse_freq_comp(coarse_freq1[0],phase_count)
			     *symbol1[i+zeros_on_left+coarse_freq1[0]])/hestimate1[0][i];

			out[i+1] = (coarse_freq_comp(coarse_freq1[0],phase_count)
		   	     *symbol1[i+1+zeros_on_left+coarse_freq1[0]])/hestimate1[0][i+1];
	       }
	     
	   }
	   else if(second_preamble) {
		     correlate2(&symbol1[0], &symbol2[0], zeros_on_left);
		     calculate_equalizer(1, &symbol1[0], &symbol2[0], zeros_on_left);
		     second_preamble = false;

		     //equalize the 2nd preamble
		     for(unsigned int i = 0; i < d_occupied_carriers; i+=2) {
			 out[i] = (coarse_freq_comp(coarse_freq2[1],phase_count)
			   *symbol2[i+zeros_on_left+coarse_freq2[1]])/hestimate2[1][i];

			 out[i+1] = (coarse_freq_comp(coarse_freq2[1],phase_count)
			   *symbol2[i+1+zeros_on_left+coarse_freq2[1]])/hestimate2[1][i+1];
		       }
	     

	   }
	   else{
	    
	     for(unsigned int i = 0; i < d_occupied_carriers/2; i++) {

		 // frequency offset compensation
		gr_complex y00  = 
		    symbol1[2*i+zeros_on_left+coarse_freq1[0]]*coarse_freq_comp(coarse_freq1[0], phase_count);
		gr_complex y10 = 
		    symbol1[2*i+1+zeros_on_left+coarse_freq1[0]]*coarse_freq_comp(coarse_freq1[0],phase_count);
		gr_complex y01 = 
		    symbol2[2*i +zeros_on_left+coarse_freq2[1]] * coarse_freq_comp(coarse_freq2[1],phase_count);
		gr_complex y11 = 
		    symbol2[2*i+1 +zeros_on_left + coarse_freq2[1]] * coarse_freq_comp(coarse_freq2[1],phase_count);

	       //this is where alamouti-decoding takes palce
	       gr_complex out1 = 
			(conj(hestimate1[0][2*i])*y00 + hestimate1[1][2*i+1]*conj(y10)
			 + conj(hestimate2[0][2*i])*y01 + hestimate2[1][2*i+1]*conj(y11)); //s1^

	       gr_complex out2 = 
			(conj(hestimate1[1][2*i])*y00 - hestimate1[0][2*i+1]*conj(y10)
			 + conj(hestimate2[1][2*i])*y01 - hestimate2[0][2*i+1]*conj(y11)); //s2^

	       out[2*i] = out1;
	       out[2*i+1] = out2;
	     }
	   }

	   phase_count++;
	   if(phase_count >= MAX_NUM_SYMBOLS) 
	    	 phase_count = 1;

		// we have consumed 1 item, and generated 1 item
	   consume_each(1);
	   return 1;
    }

  } /* namespace mimo */
} /* namespace gr */

