# MIMO-OFDM with Alamouti space-time coding
**Bachelor Thesis: Implementation of a Software-Defined Radio testbed for OFDM-MIMO communication systems.**


Alireza Zaeemzadeh __ *University of Tehran* __ 2014

This module consists of blocks requirung for building a MIMO system in GNU-Radio. The system is tested on USRP1 devices.
Thanks for your interest in my code. This github project contains many different mini-projects. The one that I actually tested on hardware and worked successfully is the MIMO OFDM with Alamouti coding (which was the main project and a part of my senior design for the B.S. degree). All the other designs are starting points of other projects in the lab that I used to work in. So, all the designs with MAC layer (such as “FinalDesign”) and designs with QPSK modulation (such as “qpsk_mimo_usrp”) are not complete works and probably do not work on hardware immediately (you have to work on them a little bit). 


**This is one of my oldest repositories. So I cannot maintain it and resolve the issues since I do not have the necessary hardware and the interest to do so. If you can maintain it I will be happy to add you as a collaborator.**

## Building Instruction
```bash
 mkdir build
 cd build
 cmake ..
 make
 sudo make install
 sudo ldconfig 
```


## Testbed Specifications:
* SDR Platform: USRP1
* Daughterboard: WBX
* Channel: TX/RX
* Antenna: VERT900
* Frequency: 1900 MHz

## Directories:
* lib : C++ codes for different blocks
* grc : *.xml files for blocks
* examples: **you can find screenshots of my designs here**
* apps: *.grc files for some applications and high level blocks 

## Usefull Links:
* Installing GNURadio using PyBombs: http://gnuradio.org/redmine/projects/pybombs/wiki
* GNU Radio Manual and C++ API Reference: http://gnuradio.org/doc/doxygen/index.html
* GNU Radio Companion: http://gnuradio.org/redmine/projects/gnuradio/wiki/GNURadioCompanion
* How to create out-of-tree modules: http://gnuradio.org/redmine/projects/gnuradio/wiki/OutOfTreeModules
* How to code blocks: http://gnuradio.org/redmine/projects/gnuradio/wiki/BlocksCodingGuide
* Tagged Stream Blocks: http://gnuradio.org/doc/doxygen/page_tagged_stream_blocks.html
