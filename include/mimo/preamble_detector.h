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


#ifndef INCLUDED_MIMO_PREAMBLE_DETECTOR_H
#define INCLUDED_MIMO_PREAMBLE_DETECTOR_H

#include <mimo/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace mimo {

    /*!
     * \brief <+description of block+>
     * \ingroup mimo
     *
     */
    class MIMO_API preamble_detector : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<preamble_detector> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of mimo::preamble_detector.
       *
       * To avoid accidental use of raw pointers, mimo::preamble_detector's
       * constructor is in a private implementation
       * class. mimo::preamble_detector::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace mimo
} // namespace gr

#endif /* INCLUDED_MIMO_PREAMBLE_DETECTOR_H */

