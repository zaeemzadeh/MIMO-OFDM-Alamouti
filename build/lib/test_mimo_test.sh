#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/alireza/Desktop/GNU/gr-mimo/lib
export PATH=/home/alireza/Desktop/GNU/gr-mimo/build/lib:$PATH
export LD_LIBRARY_PATH=/home/alireza/Desktop/GNU/gr-mimo/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-mimo 
