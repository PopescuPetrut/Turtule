# Compiler setup.
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=runic

# Manually define all targets.
build: $(TARGETS)

runic: runic.c memory_functions.c runic.h undo_redo.c system_process.c
	$(CC) $(CFLAGS) runic.c memory_functions.c runic.h undo_redo.c system_process.c  LSYSTEM.c turtule.c bitcheck.c -lm -o runic

# Clean the solution.
clean:
	rm -f $(TARGETS)

.PHONY: pack clean
