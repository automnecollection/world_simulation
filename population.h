#ifndef WORLD_SIM_POPULATION_H
#define WORLD_SIM_POPULATION_H

#include <stdio.h>

#include "language.h"

struct Province;

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

struct PopulationList initialise_populations(FILE * file, struct Province *provinces, int provinces_size);
void free_the_people(struct Population populations[], int populations_num);
void print_populations_for_province(struct Population populations[], int populations_num, int province_id);
float calc_growth_factor(float urb, float col, float lit, float sec);

// SIMULATION
void increase_pop_size(struct Population *pop, float base_birth_rate);

void cmplx_increase_pop_size(struct Population *pop,
    float urbanisation, float college_education, float literacy, float secularism);

void calculate_total_population(struct Province *prov, struct Population populations[], int populations_num);

#endif //WORLD_SIM_POPULATION_H
