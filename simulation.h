#ifndef WORLD_SIM_SIMULATION_H
#define WORLD_SIM_SIMULATION_H

#include <stdio.h>

#include "population.h"
#include "province.h"
#include "item.h"
#include "world_functions.h"

struct Province;
struct Population;
struct BuildingType;
struct NaturalResource;
struct WorldTime;

void run_simulation(int sim_days, struct Province provinces[], int provinces_num, struct Country countries[], int countries_num,
                    struct Population populations[], int populations_num, struct BuildingType building_types[], struct NaturalResource *nr_types,
                    struct WorldTime *world_time);

#endif //WORLD_SIM_SIMULATION_H
