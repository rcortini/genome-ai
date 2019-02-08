#!/bin/bash

# check for proper invocation
if [ $# -ne 6 ]; then
  echo "Usage: make_dataset <name> <genome_file> <nseqs> <l> <N> <seed>" 1>&2
  exit 1
fi

# get parameters from command line
name=$1
genome_file=$2
nseqs=$3
l=$4
N=$5
seed=$6

out_fname="../data/$name-l-$l-N-$N.dataset"
./getseq/getseq $genome_file $nseqs $l $N $seed > $out_fname
