program=randlines
program_main=$(program).c

# flags
CFLAGS=-std=gnu99 -Wall -Wextra -Wshadow -g --pedantic -O0
LDFLAGS=
INCLUDES=

# programs
RM_F=rm -f
TAR=tar
CC=gcc

# targets
.PHONY: clean tarball

all : $(program)

$(program): $(program_main)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(program) $(program_main) $(program_objects) $(LDFLAGS)

# clean targets
clean :
	$(RM_F) $(program)

# tarball
tarball :
	$(TAR) -cf $(program).tar $(program_main) Makefile
