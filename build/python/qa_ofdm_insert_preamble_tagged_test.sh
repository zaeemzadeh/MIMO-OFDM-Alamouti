#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/alireza/Desktop/GNU/gr-mimo/python
export PATH=/home/alireza/Desktop/GNU/gr-mimo/build/python:$PATH
export LD_LIBRARY_PATH=/home/alireza/Desktop/GNU/gr-mimo/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/alireza/Desktop/GNU/gr-mimo/build/swig:$PYTHONPATH
/usr/bin/python2 /home/alireza/Desktop/GNU/gr-mimo/python/qa_ofdm_insert_preamble_tagged.py 
