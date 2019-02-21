#!/bin/bash

function time_string {
  echo "[`date +"%Y-%m-%d %H:%M:%S"`]"
}

function log_message {
  echo "`time_string` `basename $0`: INFO: $1"
}

# check for proper invocation
if [ $# -ne 4 ]; then
  echo >&2 "Usage: make_sequences.sh <input_file> <nlines> <promoter_min_counts> <genome_file>"
	exit 1
fi

# get parameters from command line
input_file=$1
nlines=$2
promoter_min_count=$3
genome_file=$4

# use randlines to extract the requested number of random lines from the input
# file
rand_sample="../../data/rand_sample-SuRE.dat"
head -n 1 $input_file > $rand_sample
log_message "Extracting random sample"
./randlines $input_file $nlines $promoter_min_count $RANDOM >> $rand_sample

# now use the python magic to extract the sequences corresponding to the sample
log_message "Converting random sample to sequences"
rand_sequences="../../data/rand_sequences-SuRE.dat"
python3 extract_sequences.py $rand_sample $promoter_min_count $genome_file > $rand_sequences

log_message "Done"
