// Popescu Petrut - Alin 312CA
#include "runic.h"

// This is the most important function i like to call the "GOD FUNCTION",
// it is processing all the commands that are provided in the terminal and
// process the command using auxiliar functions
void process_command(char *command, LSYSTEM *current_ls, MANAGER *undo_manager,
					 MANAGER *redo_manager, PPM *current_ppm)
{
	if (strstr(command, "LSYSTEM")) {
		LSYSTEM_PROCESS(command, current_ls, undo_manager, redo_manager,
						current_ppm);

	} else if (strstr(command, "DERIVE")) {
		DERIVE_PROCESS(command, current_ls);

	} else if (strcmp(command, "UNDO") == 0) {
		UNDO_PROCESS(undo_manager, redo_manager, current_ls, current_ppm);

	} else if (strcmp(command, "REDO") == 0) {
		REDO_PROCESS(redo_manager, undo_manager, current_ls, current_ppm);

	} else if (strstr(command, "LOAD")) {
		PPM new_image = {0, 0, 0, NULL};

		char *way = string_allocation(MAXDIM);
		if (!way) {
			return;
		}
		strcpy(way, command + 5);

		if (load_PPM_file(way, &new_image) == 1) {
			printf("Loaded %s (PPM image %dx%d)\n",
				   way, new_image.nrcolums, new_image.nrrows);

			push(undo_manager, copy(*current_ls),
				 copy_ppm(*current_ppm), CMD_LOAD);
			clear_MANAGER(redo_manager);

			if (current_ppm->image) {
				free_ppm(current_ppm);
			}

			*current_ppm = new_image;
		} else {
			printf("Failed to load %s\n", way);
		}

		free(way);
	} else if (strstr(command, "SAVE")) {
		char *way = string_allocation(MAXDIM);
		if (!way) {
			return;
		}
		strcpy(way, command + 5);

		if (SAVE(way, current_ppm) == 1) {
			printf("Saved %s\n", way);
		} else {
			printf("No image loaded");
		}
		free(way);
	} else if (strstr(command, "TURTLE")) {
		if (!current_ls->axi) {
			printf(" No L-system loaded\n");
		} else if (!current_ppm->image) {
			printf("No image loaded");
		} else {
			printf("Drawing done\n");
			push(undo_manager, copy(*current_ls),
				 copy_ppm(*current_ppm), CMD_TURTLE);
			clear_MANAGER(redo_manager);
			turtle(command, current_ls, current_ppm);
		}
	} else if (strcmp(command, "BITCHECK") == 0) {
		if (!current_ppm->image) {
			printf("No image loaded\n");
		} else {
			bitcheck(current_ppm);
		}
	}
}
