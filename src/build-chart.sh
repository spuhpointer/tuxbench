#!/bin/bash

#
# @(#) Plot the results
#

export NDRX_BENCH_FILE=`pwd`/result.txt

#
# Generate the chart
#
export NDRX_BENCH_TITLE="Service call (one server calls one client)"
export NDRX_BENCH_X_LABEL="Msg Size (bytes)"
export NDRX_BENCH_Y_LABEL="Calls Per Second (tpcall()/sec)"
export NDRX_BENCH_OUTFILE="endurox.png"
R -f genchart.r



