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

#include <stdio.h>
#include <gnuradio/io_signature.h>
#include <fstream> 
#include "alamouti_decode_cc_impl.h"

#define PREAMBLE1 3
#define PREAMBLE2 4
#define PILOT1 1
#define PILOT2 2
#define DATA 0

namespace gr {
  namespace mimo {

    alamouti_decode_cc::sptr
    alamouti_decode_cc::make()
    {
      return gnuradio::get_initial_sptr
        (new alamouti_decode_cc_impl());
    }

    /*
     * The private constructor
     */
    alamouti_decode_cc_impl::alamouti_decode_cc_impl()
      : gr::block("alamouti_decode_cc",
              gr::io_signature::make(2, 2, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
	{
		//TODO: make these available in GRC
		payload_length = 4*512;		// in symbols
		pilot_length = 128;		// in symbols

		state = PILOT1;
		pilot_symbol[0] = gr_complex(1,-1);//constelleation dependent
		pilot_symbol[1] = gr_complex(-1,1);
		pilot_symbol[2] = gr_complex(1,1);
		pilot_symbol[3] = gr_complex(-1,-1);
		// initialization
		symbols_recieved = 0;
		pilots_recieved = 0;
		rcvd_pilot_rx0 = gr_complex(0,0);
		rcvd_pilot_rx1 = gr_complex(0,0);
		h00 = 1;
		h01 = 1;
		h10 = 1;
		h11 = 1;
		norm = h00*conj(h00) + h01*conj(h01) + h10*conj(h10) + h11*conj(h11);
		log.open("log_decoder.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		// make output buffer multiple of 2
		set_output_multiple(2);	
		// this line is not necessaery at all
		set_relative_rate(payload_length/(payload_length + pilot_length));
	}

    /*
     * Our virtual destructor.
     */
    alamouti_decode_cc_impl::~alamouti_decode_cc_impl()
    {
	log.close();
    }

    void
    alamouti_decode_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
	// in alamouti algorithm, every 2 inputs produce 2 outputs. So:
        ninput_items_required[0] = noutput_items; 
        ninput_items_required[1] = noutput_items;
    }
	
	void
	alamouti_decode_cc_impl::recieve_pilot (int &consumed_inputs,const int noutput_items,
			const gr_complex *in0, const gr_complex *in1) 
	{
		// as long as we have items in input buffer we process symbols
		for (;consumed_inputs < noutput_items; consumed_inputs++){
			if (pilots_recieved == pilot_length ){
				//after recieving a set of pilots
				pilots_recieved = 0;
				rcvd_pilot_rx0 = rcvd_pilot_rx0/gr_complex(pilot_length,0);//averaging
				rcvd_pilot_rx1 = rcvd_pilot_rx1/gr_complex(pilot_length,0);
				if (state == PILOT1){
					state = PILOT2;//we change the state of reciever
					h00 = rcvd_pilot_rx0;//calculating
					h01 = rcvd_pilot_rx1;
				} else if(state == PILOT2){
					state = DATA;
					h10 = rcvd_pilot_rx0;
					h11 = rcvd_pilot_rx1;
				}
				norm = h00*conj(h00) + h01*conj(h01) + h10*conj(h10) + h11*conj(h11);
				// reseting variables
				rcvd_pilot_rx0 = gr_complex(0,0);
				rcvd_pilot_rx1 = gr_complex(0,0);
				break;
			}
			// summing channel estimations
			rcvd_pilot_rx0 += in0[consumed_inputs]/pilot_symbol[pilots_recieved%4];
			rcvd_pilot_rx1 += in1[consumed_inputs]/pilot_symbol[pilots_recieved%4];
			pilots_recieved++;
		}
		return;
	}

    int
    alamouti_decode_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
	// input and output buffer pointers
        const gr_complex *in0 = (const gr_complex *) input_items[0];
        const gr_complex *in1 = (const gr_complex *) input_items[1];
        gr_complex *out = (gr_complex *) output_items[0];
	// initialization
	int generated_outputs = 0;
	int consumed_inputs = 0;

	if (state == PILOT1 && pilot_length != 0 ){ //in pilot state TX0
		this->recieve_pilot (consumed_inputs,noutput_items,in0,in1);
	}

	if (state == PILOT2 && pilot_length != 0 ){ //in pilot state TX1
		this->recieve_pilot (consumed_inputs,noutput_items,in0,in1);		
	}
	
	
	if (state == DATA || pilot_length == 0)
		// as long as we have items in input buffer we process symbols
		for(;consumed_inputs < noutput_items; consumed_inputs+=2) {
			if (symbols_recieved == payload_length){// it's time to recieve pilots again
				state = PILOT1;
				symbols_recieved = 0;
				break; // in next function callback pilot symbols will be sent
			}
			//MIMO Space-Time Block Coding (STBC):

			int i = consumed_inputs; // index of current input in buffer
			int o = generated_outputs; // index of current output in buffer

			out[o]   = conj(h00)*in0[i] + h10*conj(in0[i+1]) + conj(h01)*in1[i] + h11*conj(in1[i+1]) ;
			out[o+1] = conj(h10)*in0[i] - h00*conj(in0[i+1]) + conj(h11)*in1[i] - h01*conj(in1[i+1]);
			out[o]   = out[o]/norm;
			out[o+1] = out[o+1]/norm;
			
			generated_outputs +=2;//outputs in this call
			symbols_recieved +=2;//total payload symbols
			//log << out[i] << '\t' << in0[i] << '\t' << in1[i] << '\n';
			//log << out[i+1] << '\t' << in0[i+1] << '\t' << in1[i+1] << '\n';
			
	   	}
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (consumed_inputs);

        // Tell runtime system how many output items we produced.
        return generated_outputs;
    }

  } /* namespace mimo */
} /* namespace gr */

