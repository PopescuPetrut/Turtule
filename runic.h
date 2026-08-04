// Popescu Petrut - Alin 312CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PI 3.14159265358979323846
#define CMD_LSYSTEM 1
#define CMD_LOAD 2
#define CMD_TURTLE 3
#define MAXDIM 1000
#define MIDDLEDIM 10
#define UC unsigned char

typedef struct {
	char *axi;
	char **rules;
	int nrrules;
	char *way;
} LSYSTEM;

typedef struct {
	char R;
	char G;
	char B;
} PIXEL;

typedef struct {
	int nrrows;
	int nrcolums;
	int maxdim;
	PIXEL **image;
} PPM;

typedef struct {
	double pos_x;
	double pos_y;
	int orientation;
} TURTLE;

typedef struct {
	int top;
	int dimention;
	TURTLE *stack;
} TURTLE_HISTORY;

typedef struct {
	LSYSTEM *ls;
	PPM *image;
	int *cmd;
	int top;
	int height;
} MANAGER;

char *string_allocation(int dim);
void process_command(char *command, LSYSTEM *current_ls, MANAGER *undo_manager,
					 MANAGER *redo_manager, PPM *current_ppm);
int load_from_file(char *way, LSYSTEM *ls);
LSYSTEM copy(LSYSTEM a);
void push(MANAGER *stack, LSYSTEM element, PPM imag, int code);
void free_LSYSTEM(LSYSTEM *sys);
void clear_MANAGER(MANAGER *man);
void destroy_MANAGER(MANAGER *man);
int load_PPM_file(char *way, PPM *ppm);
void free_ppm(PPM *image);
int SAVE(char *way, PPM *ppm);
void DERIVE(int number, LSYSTEM *current_ls, char **dervated);
void draw_line(double start_x, double start_y, double stop_x, double stop_y,
			   PPM *image, PIXEL color);
void turtle(char *command, LSYSTEM *current_ls, PPM *current_ppm);
PPM copy_ppm(PPM a);
void bitcheck(PPM *ppm);
void LSYSTEM_PROCESS(char *command, LSYSTEM *current_ls, MANAGER *undo_manager,
					 MANAGER *redo_manager, PPM *current_ppm);
void DERIVE_PROCESS(char *command, LSYSTEM *current_ls);
void UNDO_PROCESS(MANAGER *undo_manager, MANAGER *redo_manager,
				  LSYSTEM *current_ls, PPM *current_ppm);
void REDO_PROCESS(MANAGER *redo_manager, MANAGER *undo_manager,
				  LSYSTEM *current_ls, PPM *current_ppm);
void LSYSTEM_PROCESS(char *command, LSYSTEM *current_ls, MANAGER *undo_manager,
					 MANAGER *redo_manager, PPM *current_ppm);
