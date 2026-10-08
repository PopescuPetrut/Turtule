// Popescu Petrut - Alin
#include "runic.h"

// This function saved a ppm image in a file
int SAVE(char *way, PPM *ppm)
{
	if (!way) {
		return 0;
	}

	FILE *file = fopen(way, "wb");
	if (!file) {
		fprintf(stderr, "File opening failed");
		return 0;
	}

	fprintf(file, "P6\n");
	fprintf(file, "%d %d\n", ppm->nrcolums, ppm->nrrows);
	fprintf(file, "%d\n", ppm->maxdim);

	for (int i = ppm->nrrows - 1; i >= 0; i--) {
		fwrite(ppm->image[i], sizeof(PIXEL), ppm->nrcolums, file);
	}
	fclose(file);
	return 1;
}

// Verify if the (x,y) element is inside of a matrix with (n, m) dimenssion
int is_inside(int x, int y, int n, int m)
{
	if (x < n && x >= 0 && y < m && y >= 0) {
		return 1;
	}
	return 0;
}

// Implements the Bresenham algorithm provided in the file
void draw_line(double x0, double y0, double x1, double y1,
			   PPM *image, PIXEL color)
{
	int start_x = (int)round(x0);
	int start_y = (int)round(y0);
	int stop_x = (int)round(x1);
	int stop_y = (int)round(y1);

	int dx = abs(stop_x - start_x);
	int sx = -1;
	if (start_x < stop_x) {
		sx = 1;
	}

	int dy = -abs(start_y - stop_y);
	int sy = -1;
	if (start_y < stop_y) {
		sy = 1;
	}

	int err = dx + dy;

	while (1) {

		if (is_inside(start_x, start_y, image->nrcolums, image->nrrows)) {
			image->image[start_y][start_x] = color;
		}

		if (start_x == stop_x && start_y == stop_y) {
			break;
		}

		int err2 = 2 * err;
		if (err2 >= dy) {
			err += dy;
			start_x += sx;
		}
		if (err2 <= dx) {
			err += dx;
			start_y += sy;
		}
	}
}

// This function resolves the turtle command
void turtle(char *command, LSYSTEM *current_ls, PPM *current_ppm)
{
	TURTLE tur = {0, 0, 0};
	int dist = 0;
	int angle_add = 0;
	int n = 0;
	PIXEL collor = {0, 0, 0};
	char *p = strtok(command, " ");
	p = strtok(NULL, " ");
	tur.pos_x = atof(p);
	p = strtok(NULL, " ");
	tur.pos_y = atof(p);
	p = strtok(NULL, " ");
	dist = atoi(p);
	p = strtok(NULL, " ");
	tur.orientation = atoi(p);
	p = strtok(NULL, " ");
	angle_add = atoi(p);
	p = strtok(NULL, " ");
	n = atoi(p);
	p = strtok(NULL, " ");
	collor.R = atoi(p);
	p = strtok(NULL, " ");
	collor.G = atoi(p);
	p = strtok(NULL, " ");
	collor.B = atoi(p);

	char *steps = NULL;
	DERIVE(n, current_ls, &steps);
	if (!steps) {
		return;
	}
	int nr_steps = strlen(steps);
	TURTLE_HISTORY stack = {0, MAXDIM, NULL};
	stack.stack = (TURTLE *)malloc(MAXDIM * sizeof(TURTLE));
	if (!stack.stack) {
		fprintf(stderr, "Stack allocation error");
		return;
	}

	for (int i = 0; i < nr_steps; i++) {
		if (steps[i] == 'F') {

			double rad = (double)(tur.orientation * PI / 180);
			double next_x = tur.pos_x + (double)(dist) * cos(rad);
			double next_y = tur.pos_y + (double)(dist) * sin(rad);

			draw_line(tur.pos_x, tur.pos_y, next_x, next_y,
					  current_ppm, collor);

			tur.pos_x = next_x;
			tur.pos_y = next_y;
		} else if (steps[i] == '+') {
			tur.orientation = (tur.orientation + angle_add) % 360;
		} else if (steps[i] == '-') {
			if (tur.orientation - angle_add < 0) {
				tur.orientation += 360;
			}
			tur.orientation = (tur.orientation - angle_add) % 360;
		} else if (steps[i] == '[') {
			if (stack.top == stack.dimention) {
				TURTLE *aux = realloc(stack.stack, 2 * stack.dimention
										* sizeof(TURTLE));
				if (!aux) {
					fprintf(stderr, "Stack realocation failed");
					return;
				}
				stack.stack = aux;
			}
			stack.stack[stack.top] = tur;
			stack.top++;
		} else if (steps[i] == ']') {
			if (stack.top > 0) {
				stack.top--;
				tur = stack.stack[stack.top];
			}
		}
	}

	free(steps);
	free(stack.stack);
}
