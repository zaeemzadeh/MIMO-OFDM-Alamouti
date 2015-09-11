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

#ifndef INCLUDED_MIMO_OFDM_DEMAPPER_IMPL_H
#define INCLUDED_MIMO_OFDM_DEMAPPER_IMPL_H

#include <mimo/ofdm_demapper.h>
#include <fstream>
#include <gnuradio/tags.h>

namespace gr {
  namespace mimo {

    class ofdm_demapper_impl : public ofdm_demapper
    {
     private:
	  std::fstream log;
	  enum state_t {STATE_SYNC_SEARCH, STATE_HAVE_SYNC, STATE_HAVE_FIRST_SYMBOL, STATE_HAVE_HEADER};
	  state_t state;

	  static const int MAX_PKT_LEN = 512;
	  static const int HEADERBYTELEN   = 4;
	  static const int HEADERCONSTPOINTS = HEADERBYTELEN*8/2; 
	
	  unsigned int       d_occupied_carriers;

	  unsigned int       header;                 

	  int                packetlen;               //packet length in qpsk symbols
	  int                packetlen_cnt;           //counter of extracted qpsk symbols

	  std::vector<int>   subcarrier_map;          

	  unsigned int               bits_per_symbol;        
	  std::vector<gr_complex>    constell;   

	  float phase;
	  float freq;
	  float phase_gain;
	  float freq_gain;
	  float eq_gain;

	  //functions

	  bool header_ok();

	  void enter_search();
	  void enter_have_sync();
	  void enter_have_header();
	  void enter_have_first_symbol();

	  unsigned char slicer(const gr_complex x);
	  void demodulate_header(const gr_complex *in);
     public:
      ofdm_demapper_impl(unsigned int occupied_carriers);
      ~ofdm_demapper_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace mimo
} // namespace gr

#endif /* INCLUDED_MIMO_OFDM_DEMAPPER_IMPL_H */

