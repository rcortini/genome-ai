#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned valid_DNA_char[256] = { 
   0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0, // 0-15
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 16-31
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 32-47
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 48-63
   0,1,0,1,0,0,0,1,0,0,0,0,0,0,1,0, // 64-79
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, // 80-95
   0,1,0,1,0,0,0,1,0,0,0,0,0,0,1,0, // 96-111
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
 * This table maps a single character to a number
 */
static const unsigned mapping[256] = {
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 0-15
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 16-31
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 32-47
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 48-63
   0,0,0,2,0,0,0,3,0,0,0,0,0,0,4,0, // 64-79
   0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0, // 80-95
   0,0,0,2,0,0,0,3,0,0,0,0,0,0,4,0, // 96-111
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
    b = 5*b;
  }
  return encoding;
}

int main(int argc, char **argv) {

  // check for proper invocation
  if (argc != 3) {
    fprintf(stderr, "Usage: getseq <fname> <nletters>\n");
    return EXIT_FAILURE;
  }

  // parse command line arguments
  char *fin = argv[1];
  size_t nletters = atoi(argv[2]);

  // initialize the variables that we need for the iteration
  size_t word_encoding;
  char word[nletters+1];
	FILE *f;

	// open the input file
	if (strcmp(fin, "-") == 0)
		f = stdin;
	else
		f = fopen(fin, "r");

  // do the iteration over all the lines in the file
	size_t i = 0;
	int c;
  while ((c = fgetc(f)) != EOF) {

		// let's make sure that we have a valid DNA character
		if(!valid_DNA_char[c]) {
			fprintf(stderr, "ERROR: invalid character encountered: %c\n", c);
			exit(1);
		}

		// if we encounter a newline, go to next character
		if (c == '\n' || feof(f)) {
			printf("\n");
			continue;
		}

		// if we arrive here then we can store the character and increment i
		word[i%nletters] = (char) c;
		i++;

		// if we completed a word then i%nletters will be equal to zero
		if (i%nletters == 0) {
			word[nletters] = '\0';
			word_encoding = word_to_encoding(word, nletters);
			printf("%lu ", word_encoding);
		}
  }

  // clean up and exit
  fclose(f);
  return 0;
}
