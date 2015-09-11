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

#ifndef INCLUDED_MIMO_ALAMOUTI_OFDM_RX_IMPL_H
#define INCLUDED_MIMO_ALAMOUTI_OFDM_RX_IMPL_H

#include <mimo/alamouti_ofdm_rx.h>
#include <vector>
#include <fstream>

namespace gr {
  namespace mimo {

    class alamouti_ofdm_rx_impl : public alamouti_ofdm_rx
    {
    private:
	//block parameters
	unsigned int d_occupied_carriers;       
	unsigned int d_fft_length;              
	unsigned int d_cplen;                   

	//log file
	std::fstream log;		


	unsigned int freq_shift_len;          // number of surrounding subcarriers to look at for correlation
	std::vector<gr_complex> known_symbol0;		//preamble symbol #1
	std::vector<gr_complex> known_symbol1;		//preamble symbol #2

	std::vector<float> known_phase_diff1;  		//phase diff in preamble #1
	std::vector<float> known_phase_diff2;  		//phase diff in preamble #2

	std::vector<float> symbol_phase_diff1; 		//phase diff in recieved preamble #1
	std::vector<float> symbol_phase_diff2; 		//phase diff in recieved preamble #2

	std::vector<gr_complex> *hestimate1;  // channel estimate for first rx antenna
	std::vector<gr_complex> *hestimate2;  //  channel estimate for second rx antenna
	int *coarse_freq1;                     // coarese frequency offset estimation for first antenna
	int *coarse_freq2;                     // coarese frequency offset estimation for second antenna
	unsigned int phase_count ;    	

	// state
	bool first_preamble;
	bool second_preamble;   

	//functions
	void correlate1(const gr_complex *symbol0, const gr_complex *symbol1, int zeros_on_left);
	void correlate2(const gr_complex *symbol0, const gr_complex *symbol1, int zeros_on_left);
	void calculate_equalizer(int channel, const gr_complex *symbol1, const gr_complex *symbol2, int zeros_on_left);
	gr_complex coarse_freq_comp(int freq_delta, int count);

     public:
      alamouti_ofdm_rx_impl(unsigned int occupied_carriers, unsigned int fft_length, unsigned int cplen);
      ~alamouti_ofdm_rx_impl();
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);
      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace mimo
} // namespace gr

#endif /* INCLUDED_MIMO_ALAMOUTI_OFDM_RX_IMPL_H */

