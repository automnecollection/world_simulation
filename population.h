#include <stdio.h>

#ifndef WORLD_SIM_POPULATION_H
#define WORLD_SIM_POPULATION_H

struct Population {
    int id;
    int province_id;
    float p_size;
    int p_size_int;
    char *culture;
    char *religion;
};

struct PopulationList {
    int populations_num;
    struct Population *populations;
};

struct PopulationList initialise_populations(FILE * file);
struct Population *read_population(char *line, int id);
void increase_pop_size(struct Population *pop, float BASE_BIRTH_RATE);
void free_populations(struct Population populations[], int populations_num, struct PopulationList *population_list);
void print_populations_for_province(struct Population populations[], int populations_num, int province_id);

#endif //WORLD_SIM_POPULATION_H
