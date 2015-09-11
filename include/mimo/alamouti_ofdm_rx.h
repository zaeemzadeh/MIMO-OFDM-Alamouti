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


#ifndef INCLUDED_MIMO_ALAMOUTI_OFDM_RX_H
#define INCLUDED_MIMO_ALAMOUTI_OFDM_RX_H

#include <mimo/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace mimo {

    /*!
     * \brief <+description of block+>
     * \ingroup mimo
     *
     */
    class MIMO_API alamouti_ofdm_rx : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<alamouti_ofdm_rx> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mimo::alamouti_ofdm_rx.
       *
       * To avoid accidental use of raw pointers, mimo::alamouti_ofdm_rx's
       * constructor is in a private implementation
       * class. mimo::alamouti_ofdm_rx::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int occupied_carriers, unsigned int fft_length, unsigned int cplen);
    };

  } // namespace mimo
} // namespace gr

#endif /* INCLUDED_MIMO_ALAMOUTI_OFDM_RX_H */

