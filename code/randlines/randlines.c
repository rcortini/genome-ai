#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 4096

int safe_realloc (size_t new_vector_size, char **vector) {
  char *is_null;
  is_null = (char *) realloc (*vector, new_vector_size * sizeof (char));
  if (is_null == NULL)
    return 0;
  *vector = is_null;
  return 1;
}

int always_go(char *line, void *p) {
  (void) line;
  (void) p;
  return 1;
}

size_t get_sum_counts(char *line) {
  size_t argn = 0;
  size_t c0, c1, c2, c3, c4, c5;
  size_t i = 0, j = 0;
  char *mychars = (char *) malloc(BUFF_SIZE * sizeof(char));
  do {
    if (line[i++] == '\t') {
      argn++;
    }

    // if we're in range, start reading the string
    if (argn > 4) {
      mychars[j++] = line[i];
    }

  } while (argn < 10);

  // now convert the characters to integers
  mychars[j] = '\0';
  sscanf(mychars, "%lu\t%lu\t%lu\t%lu\t%lu\t%lu", &c0, &c1, &c2, &c3, &c4, &c5);
  free(mychars);

  // compute the sum of the counts
  return c0 + c1 + c2 + c3 + c4 + c5;
}

int is_promoter(char *line, size_t line_length, void *p) {
  (void) line_length;
  size_t promoter_min_counts = (size_t) p;

  size_t sum = get_sum_counts(line);

  // if the promoter minimum counts is met, it's a promoter
  if (sum >= promoter_min_counts)
    return 1;
  else
    return 0;
}

int is_not_promoter(char *line, size_t line_length, void *p) {
  (void) line_length;
  (void) p;

  size_t sum = get_sum_counts(line);

  // if the promoter count is zero, it's not a promoter
  if (sum == 0)
    return 1;
  else
    return 0;
}

void print_random_line(
    FILE *f,
    size_t file_size,
    int (*callback) (char *line, size_t line_length, void *p),
    void *p) {

  // declare the vector of characters that we are after
  char *line = (char *) malloc(BUFF_SIZE * sizeof(char));
  size_t char_n;
  int cb;

  // we try to get a line that will make our callback function happy
  do {

    // jump to a random spot in the file
    size_t whence = (size_t) ((double) rand() /((double) RAND_MAX) * file_size);
    fseek(f, whence, SEEK_SET);

    // read the file until the newline character is found
    while(fgetc(f) != '\n'){};

    // now fetch the line
    char c;
    char_n = 0;
    do {
      c = fgetc(f);

      if (char_n >= BUFF_SIZE) {
	if (! safe_realloc (char_n + BUFF_SIZE, &line)) {
	  fprintf (stderr, "No more memory!\n");
	  exit (EXIT_FAILURE);
	}
      }

      line[char_n++] = c;
    } while(c != '\n' || feof(f));

    cb = callback(line, char_n, p);
  } while (!cb);

  // if we're here, we can print the string
  line[char_n++] = '\0';
  printf(line);

  // free the pointer
  free(line);

}

int main(int argc, char *argv[]) {

  // check for proper invocation
  if (argc != 5) {
    fprintf(stderr, "Usage: randlines <datafile> <nlines> <promoter_min_count> <seed>\n");
    return EXIT_FAILURE;
  }

  // get arguments from command line
  char *fname = argv[1];
  size_t nlines = atoi(argv[2]);
  size_t promoter_min_count = atoi(argv[3]);
  size_t seed = atoi(argv[4]);

  // determine the size of the input file
  FILE *f = fopen(fname, "r");
  fseek(f, 0L, SEEK_END);
  size_t file_size = ftell(f);

  // initialize the random number generator
  srand(seed);

  for(size_t i = 0; i<nlines; i++) {
    print_random_line(f, file_size, is_promoter, (void *) promoter_min_count);
    print_random_line(f, file_size, is_not_promoter, (void *) promoter_min_count);
  }

  fclose(f);
  return 0;
}
