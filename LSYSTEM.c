// Popescu Petrut - Alin 312CA
#include "runic.h"

// This function process the L-system command
void LSYSTEM_PROCESS(char *command, LSYSTEM *current_ls, MANAGER *undo_manager,
					 MANAGER *redo_manager, PPM *current_ppm)
{
	LSYSTEM new_ls = {NULL, NULL, 0, NULL};

	char *way = string_allocation(MAXDIM);
	strcpy(way, command + 8);

	if (load_from_file(way, &new_ls) == 1) {
		printf("Loaded %s (L-system with %d rules)\n", way, new_ls.nrrules);

		push(undo_manager, copy(*current_ls),
			 copy_ppm(*current_ppm), CMD_LSYSTEM);

		if (current_ls->axi) {
			free_LSYSTEM(current_ls);
		}

		clear_MANAGER(redo_manager);

		*current_ls = new_ls;
	} else {
		printf("Failed to load %s\n", way);
	}

	free(way);
}

// This function process derive function
void DERIVE_PROCESS(char *command, LSYSTEM *current_ls)
{
	int number = atoi(command + 7);
	if (!current_ls->axi) {
		printf("No L-system loaded\n");
	} else {
		char *final_deriv = NULL;
		DERIVE(number, current_ls, &final_deriv);
		if (!final_deriv) {
			return;
		}
		printf("%s\n", final_deriv);
		free(final_deriv);
	}
}

// This function is used to determine the L-system derivation through a complex
// process. It is the second part of "DERIVE_PROCESS" function
void DERIVE(int number, LSYSTEM *current_ls, char **derivated)
{
	char *laststep = string_allocation(strlen(current_ls->axi) + 1);
	strcpy(laststep, current_ls->axi);

	while (number--) {
		int new_len = 0;
		for (int i = 0; laststep[i]; i++) {
			int pos = -1;
			for (int j = 0; j < current_ls->nrrules; j++) {
				if (laststep[i] == current_ls->rules[j][0]) {
					pos = j;
					break;
				}
			}

			if (pos == -1) {
				new_len += 1;
			} else {
				new_len += strlen(current_ls->rules[pos] + 2);
			}
		}

		char *derivation = string_allocation(new_len + 1);
		derivation[0] = '\0';
		for (int i = 0; laststep[i]; i++) {
			int pos = -1;
			for (int j = 0; j < current_ls->nrrules; j++) {
				if (laststep[i] == current_ls->rules[j][0]) {
					pos = j;
					break;
				}
			}

			if (pos != -1) {
				strcat(derivation, current_ls->rules[pos] + 2);
			} else {
				int len = strlen(derivation);
				derivation[len] = laststep[i];
				derivation[len + 1] = '\0';
			}
		}

		free(laststep);
		laststep = derivation;
	}
	*derivated = string_allocation(strlen(laststep) + 1);
	if (!derivated) {
		return;
	}
	strcpy(*derivated, laststep);
	free(laststep);
}
