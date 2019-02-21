import pandas as pd
import sys, os
from Bio import SeqIO, Seq
from Bio.Alphabet import IUPAC

# check for proper invocation
if len(sys.argv) != 4 :
    print("Usage: extract_sequences <sequence_file> <promoter_min_count> <genome_file>", file=sys.stderr)
    sys.exit(1)

# extract parameters from command line
fname = sys.argv[1]
promoter_min_count = int(sys.argv[2])
genome_file = sys.argv[3]

# parse data file
data = pd.read_csv(fname, sep='\t')
chromosomes = data['chr'].unique()

# load the genome
genome = SeqIO.index(genome_file, 'fasta', alphabet=IUPAC.unambiguous_dna)

# build a dictionary of data points, which correspond to a chromosome
data_chr = {chromosome : data.loc[data['chr'] == chromosome] for chromosome in chromosomes}

# parse the input file one chromosome at a time
sequences = []
targets = []
for n, chromosome in enumerate(chromosomes) :
    
    # get the reads corresponding to the current chromosome
    chromosome_data = data_chr[chromosome]
    c = genome[chromosome]
    
    # iterate through the reads
    for i, row in enumerate(chromosome_data.iterrows()) :
        
        # parse the information in the row
        r = row[1]
        start, end, strand = r['start'], r['end'], r['strand']
        myseq = c.seq[start:end]
        if strand == '-' :
            myseq = myseq.reverse_complement()
        myseq = myseq.upper()
        
        # let's figure out whether the sequence was classified as promoter or not
        counts = sum(row[1][5:11])
        if counts < 10 :
            promoter = 0
        else :
            promoter = 1
        
        # encode the sequence in the format that will be comprehensible by the AI
        sequences.append(myseq)
        targets.append(promoter)

# output
for sequence, target in zip(sequences, targets) :
    sys.stdout.write("%d %s\n"%(target, sequence))
