# Copyright PCLP Team, 2025

# Compiler setup.
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# Define targets, e.g., ninel, codeinvim, vectsecv, nomogram.
TARGETS=runic

# Manually define all targets.
build: $(TARGETS)

runic: runic.c memory_functions.c runic.h undo_redo.c system_process.c
	$(CC) $(CFLAGS) runic.c memory_functions.c runic.h undo_redo.c system_process.c  LSYSTEM.c turtule.c bitcheck.c -lm -o runic

# Pack the solution into a zip file.
pack:
	zip -FSr 312CA_PetrutAlinPopescu_Tema3.zip README Makefile *.c *.h

# Clean the solution.
clean:
	rm -f $(TARGETS)

.PHONY: pack clean
