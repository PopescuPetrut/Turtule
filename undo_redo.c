// Popescu Petrut - Alin 312CA
#include "runic.h"

// Process the undo function
void UNDO_PROCESS(MANAGER *undo_manager, MANAGER *redo_manager,
				  LSYSTEM *current_ls, PPM *current_ppm)
{
	if (undo_manager->top > 0) {
		int undo_code = undo_manager->cmd[undo_manager->top - 1];
		push(redo_manager, copy(*current_ls),
			 copy_ppm(*current_ppm), undo_code);
		free_LSYSTEM(current_ls);
		free_ppm(current_ppm);
		*current_ls = undo_manager->ls[undo_manager->top - 1];
		*current_ppm = undo_manager->image[undo_manager->top - 1];
		undo_manager->top--;
	} else {
		printf("Nothing to undo\n");
	}
}

// Process the redo function
void REDO_PROCESS(MANAGER *redo_manager, MANAGER *undo_manager,
				  LSYSTEM *current_ls, PPM *current_ppm)
{
	if (redo_manager->top > 0) {
		int restored_code = redo_manager->cmd[redo_manager->top - 1];
		push(undo_manager, copy(*current_ls),
			 copy_ppm(*current_ppm), restored_code);
		free_LSYSTEM(current_ls);
		free_ppm(current_ppm);
		*current_ls = redo_manager->ls[redo_manager->top - 1];
		*current_ppm = redo_manager->image[redo_manager->top - 1];
		redo_manager->top--;

		if (restored_code == CMD_LSYSTEM) {
			printf("Loaded %s (L-system with %d rules)\n",
				   current_ls->way, current_ls->nrrules);
		} else if (restored_code == CMD_TURTLE) {
			printf("Drawing done\n");
		} else if (restored_code == CMD_LOAD) {
			printf("Loaded %s (PPM image %dx%d)\n",
				   current_ls->way, current_ppm->nrcolums,
				   current_ppm->nrrows);
		}

	} else {
		printf("Nothing to redo\n");
	}
}

// We are using a FIFO structure to implement the undo and redo funtions
// and this function allocates new memory and add a new element in the
// structure
void push(MANAGER *stack, LSYSTEM element, PPM imag, int code)
{
	if (stack->height == 0) {
		stack->height = MIDDLEDIM;
		stack->ls = (LSYSTEM *)malloc(stack->height * sizeof(LSYSTEM));
		if (!stack->ls) {
			fprintf(stderr, "Allocation error");
			return;
		}
		stack->image = (PPM *)malloc(stack->height * sizeof(PPM));
		if (!stack->image) {
			fprintf(stderr, "Allocation error");
			return;
		}
		stack->cmd = (int *)malloc(stack->height * sizeof(int));
		if (!stack->cmd) {
			fprintf(stderr, "Allocation error");
			return;
		}
	}

	if (stack->top == stack->height) {
		stack->height *= 2;
		LSYSTEM *aux_ls = (LSYSTEM *)realloc(stack->ls, stack->height *
										  sizeof(LSYSTEM));
		if (!aux_ls) {
			fprintf(stderr, "Stack expansion failed");
			return;
		}
		stack->ls = aux_ls;

		PPM *aux_ppm = (PPM *)realloc(stack->image, stack->height
									  * sizeof(PPM));
		if (!aux_ppm) {
			fprintf(stderr, "Stack expansion failed");
			return;
		}
		stack->image = aux_ppm;

		int *aux_cmd = (int *)realloc(stack->cmd, stack->height * sizeof(int));
		if (!aux_cmd) {
			fprintf(stderr, "Stack expansion failed");
			return;
		}
		stack->cmd = aux_cmd;
	}

	stack->cmd[stack->top] = code;
	stack->image[stack->top] = imag;
	stack->ls[stack->top] = element;
	stack->top++;
}
