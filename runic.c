// Popescu Petrut - Alin
#include "runic.h"

int main(void)
{
	// declaring all the buffers that are needded to implement tge functions
	LSYSTEM sys = {NULL, NULL, 0, NULL};
	MANAGER undo_manag = {NULL, NULL, NULL, 0, MIDDLEDIM};
	MANAGER redo_manag = {NULL, NULL, NULL, 0, MIDDLEDIM};
	undo_manag.ls = (LSYSTEM *)malloc(MIDDLEDIM * sizeof(LSYSTEM));
	undo_manag.image = (PPM *)malloc(MIDDLEDIM * sizeof(PPM));
	undo_manag.cmd = (int *)malloc(MIDDLEDIM * sizeof(int));
	redo_manag.ls = (LSYSTEM *)malloc(MIDDLEDIM * sizeof(LSYSTEM));
	redo_manag.image = (PPM *)malloc(MIDDLEDIM * sizeof(PPM));
	redo_manag.cmd = (int *)malloc(MIDDLEDIM * sizeof(int));
	PPM image = {0, 0, 0, NULL};

	char *command = string_allocation(MAXDIM);
	if (!command) {
		return -1;
	}

	// loading the command in the memory and processing it
	fgets(command, MAXDIM, stdin);
	command[strlen(command) - 1] = '\0';
	while (strcmp(command, "EXIT")) {
		process_command(command, &sys, &undo_manag, &redo_manag, &image);
		fgets(command, MAXDIM, stdin);
		command[strlen(command) - 1] = '\0';
	}

	// freeing all the memory allocated during the run
	free(undo_manag.cmd);
	free(redo_manag.cmd);
	free(command);
	free_LSYSTEM(&sys);
	destroy_MANAGER(&undo_manag);
	destroy_MANAGER(&redo_manag);
	free_ppm(&image);

	return 0;
}
