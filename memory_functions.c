// Popescu Petrut - Alin 312CA
#include "runic.h"

char *string_allocation(int dim)
{
	char *s = (char *)malloc(dim * sizeof(char));
	if (!s) {
		fprintf(stderr, "String allocation error");
		return NULL;
	}

	return s;
}

char **list_of_string_alloc(int nrwords, int wordlen)
{
	char **mat = (char **)malloc(nrwords * sizeof(char *));
	if (!mat) {
		fprintf(stderr, "Allocation error");
		return NULL;
	}
	for (int i = 0; i < nrwords; i++) {
		mat[i] = (char *)malloc(wordlen * sizeof(char));
		if (!mat[i]) {
			fprintf(stderr, "Allocation error");
			for (int j = 0; j < i; j++) {
				free(mat[j]);
			}
			free(mat);
			return NULL;
		}
	}

	return mat;
}

// Loading the L-system from the provided file into the memory
int load_from_file(char *way, LSYSTEM *ls)
{
	FILE *file = fopen(way, "rt");
	if (!file) {
		return 0;
	}

	char temp[MAXDIM];
	fgets(temp, MAXDIM, file);
	temp[strcspn(temp, "\r\n")] = '\0';

	ls->axi = string_allocation(strlen(temp) + 1);
	strcpy(ls->axi, temp);
	fscanf(file, "%d", &ls->nrrules);
	fgetc(file);

	ls->rules = list_of_string_alloc(ls->nrrules, 25);

	for (int i = 0; i < ls->nrrules; i++) {
		fgets(temp, MAXDIM, file);
		temp[strcspn(temp, "\r\n")] = '\0';
		strcpy(ls->rules[i], temp);
	}

	ls->way = string_allocation(strlen(way) + 1);
	strcpy(ls->way, way);

	fclose(file);
	return 1;
}

// This function is used to deep - copy a L-system and return the value
LSYSTEM copy(LSYSTEM a)
{
	LSYSTEM neutral = {NULL, NULL, 0, NULL};
	LSYSTEM b = neutral;

	if (!a.axi) {
		return neutral;
	}

	b.nrrules = a.nrrules;
	b.axi = string_allocation(strlen(a.axi) + 1);
	if (!b.axi) {
		fprintf(stderr, "Deep-copy error");
		return neutral;
	}
	strcpy(b.axi, a.axi);
	b.rules = list_of_string_alloc(a.nrrules, MAXDIM);
	if (!b.rules) {
		fprintf(stderr, "Deep-copy error");
		return neutral;
	}
	for (int i = 0; i < a.nrrules; i++) {
		strcpy(b.rules[i], a.rules[i]);
	}
	b.way = string_allocation(strlen(a.way) + 1);
	if (!b.way) {
		fprintf(stderr, "Deep-copy error");
		return neutral;
	}
	strcpy(b.way, a.way);

	return b;
}

// This function clear a L-system. This function is needed to delete all the
// useless data
void free_LSYSTEM(LSYSTEM *sys)
{
	if (sys->axi) {
		free(sys->axi);
		sys->axi = NULL;
	}

	if (sys->way) {
		free(sys->way);
		sys->way = NULL;
	}

	if (sys->rules) {
		for (int i = 0; i < sys->nrrules; i++) {
			free(sys->rules[i]);
		}
		free(sys->rules);
		sys->rules = NULL;
	}
	sys->nrrules = 0;

}

// This function clear the manager from useless data but doesn't deallocates
// "the beginning" of the manager allowing us to re-use the same buffer not
// not needing to reallocate a new manager
void clear_MANAGER(MANAGER *man)
{
	if (!man->ls || !man || !man->image) {
		return;
	}

	for (int i = 0; i < man->top; i++) {
		free_LSYSTEM(&man->ls[i]);
		free_ppm(&man->image[i]);
	}

	man->top = 0;
}

// This function clear the manager buffer entirely, the destroyed manager
// can't be reused without a new allocation
void destroy_MANAGER(MANAGER *man)
{
	clear_MANAGER(man);
	if (man->ls) {
		free(man->ls);
		free(man->image);
		man->image = NULL;
		man->ls = NULL;
	}
	man->height = 0;
}

// This function is used to allocate a matrix of pixel that is used to storage
// an image
PIXEL **map_alloc(int n, int m)
{
	PIXEL **map;
	map = (PIXEL **)malloc(n * sizeof(PIXEL *));
	if (!map) {
		fprintf(stderr, "Map allocation error");
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		map[i] = (PIXEL *)malloc(m * sizeof(PIXEL));
		if (!map[i]) {
			fprintf(stderr, "Map allocation error");
			for (int j = 0; j < i; j++) {
				free(map[j]);
			}
			free(map);
			return NULL;
		}
	}

	return map;
}

// Here we are defining a function which loads in the memory the ppm format
// image
int load_PPM_file(char *way, PPM *ppm)
{
	FILE *file = fopen(way, "rb");
	if (!file) {
		fprintf(stderr, "PPM file loading failed");
		return 0;
	}

	char magic[3];
	fscanf(file, "%s", magic);
	fgetc(file);

	fscanf(file, "%d%d", &ppm->nrcolums, &ppm->nrrows);
	fgetc(file);
	fscanf(file, "%d", &ppm->maxdim);
	fgetc(file);

	ppm->image = map_alloc(ppm->nrrows, ppm->nrcolums);
	if (!ppm->image) {
		return 0;
		fclose(file);
	}

	for (int i = 0; i < ppm->nrrows; i++) {
		fread(ppm->image[i], sizeof(PIXEL), ppm->nrcolums, file);
	}

	fclose(file);
	return 1;
}

// This function is freeing the memory from an unwanted image
void free_ppm(PPM *image)
{
	for (int i = 0; i < image->nrrows; i++) {
		free(image->image[i]);
		image->image[i] = NULL;
	}
	free(image->image);
	image->image = NULL;

	image->maxdim = 0;
	image->nrcolums = 0;
	image->nrrows = 0;
}

// Here we are deep - copying an image
PPM copy_ppm(PPM a)
{
	PPM b = {0, 0, 0, NULL};
	PPM neutral = b;

	if (!a.image) {
		return neutral;
	}

	b.maxdim = a.maxdim;
	b.nrcolums = a.nrcolums;
	b.nrrows = a.nrrows;
	b.image = map_alloc(a.nrrows, a.nrcolums);
	if (!b.image) {
		return neutral;
	}

	for (int i = 0; i < a.nrrows; i++) {
		for (int j = 0; j < a.nrcolums; j++) {
			b.image[i][j] = a.image[i][j];
		}
	}

	return b;
}
