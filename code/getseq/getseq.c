#include <stdio.h>
#include <stdlib.h>

static const unsigned valid_DNA_char[256] = { 
   0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0, // 0-15
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 16-31
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 32-47
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 48-63
   0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0, // 64-79
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, // 80-95
   0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0, // 96-111
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, // 112-127
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 128-143
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 144-159
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 160-175
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 176-191
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 192-207
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 208-223
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 224-239
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 240-255
};

size_t random_in_range(size_t max){
   return (size_t) ((double) rand() / ((double) RAND_MAX / (max+1)+1));
}

/*
 * This function maps a single character to a number
 */
static const unsigned mapping[256] = {
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0-15
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 16-31
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 32-47
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 48-63
   0,0,0,2,0,0,0,3,0,0,0,0,0,0,0,0, // 64-79
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, // 80-95
   0,0,0,2,0,0,0,3,0,0,0,0,0,0,0,0, // 96-111
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, // 112-127
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 128-143
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 144-159
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 160-175
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 176-191
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 192-207
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 208-223
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 224-239
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 240-255
};

size_t word_to_encoding(char word[], size_t l) {
  size_t encoding = 0;
  size_t b = 1;
  for (unsigned i=l; i-- > 0 ; ) {
    char c = word[i];
    unsigned a = mapping[(int) c];
    encoding += b*a;
    b = 4*b;
  }
  return encoding;
}

int main(int argc, char **argv) {

  // check for proper invocation
  if (argc != 6) {
    fprintf(stderr, "Usage: getseq <genome> <nseqs> <l> <N> <seed>\n");
    return EXIT_FAILURE;
  }

  // parse command line arguments
  char *genome = argv[1];
  size_t nseqs = atoi(argv[2]);
  size_t l = atoi(argv[3]);
  size_t N = atoi(argv[4]);
  unsigned int seed = atoi(argv[5]);

  // determine the size of the genome file
  FILE *g = fopen(genome, "r");
  fseek(g, 0L, SEEK_END);
  size_t genome_size = ftell(g);

  // initialize the random number generator
  srand(seed);

  // initialize the variables that we need for the iteration
  size_t L = l*N;
  size_t word_encoding[N];
  char word[l+1];

  // do the iteration over all the sequences requested
  size_t n = 0;
  while (n<nseqs) {
    size_t i = 0, j = 0;

    // jump to a random spot in the file
    size_t whence = random_in_range(genome_size);
    fseek(g, whence, SEEK_SET);

    // we want to read L = l*N characters in total
    // printf("Sequence[%lu] : ", n);
    while (i<L) {
      // read a single character from the current spot
      int c = fgetc(g);

      // let's make sure that we have a valid DNA character
      if(!valid_DNA_char[c]) {
	break;
      }

      // if we encounter a newline, go to next character
      if (c == '\n')
	continue;

      // if we arrive here then we can store the character and increment i
      word[i%l] = (char) c;
      i++;

      // if we completed a word then i%l will be equal to zero
      if (i%l == 0) {
	word[l] = '\0';
        word_encoding[j] = word_to_encoding(word, l);
	// printf("%s ", word);
	j++;
      }
    }

    // a way to check that we found a valid sequence is to check whether our i
    // variable is equal to the total number of requested letters to read
    if (i == L) {

      // if we're here we can print out the encoding of the sequence we just
      // found
      for(j=0; j<N; j++)
	printf("%lu ", word_encoding[j]);
      printf("\n");

      // and finally, increment the number of sequences that we found
      n++;
    }
  }

  // clean up and exit
  fclose(g);
  return 0;
}
