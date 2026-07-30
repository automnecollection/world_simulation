#ifndef WORLD_SIM_BUILDING_H
#define WORLD_SIM_BUILDING_H

#include <stdio.h>

#include "province.h"

struct Building {
    int id;
    int level;
    int item_input_ids;
    int item_input_ids_size;
    int item_input_amounts;
    int item_input_amounts_size;
    int item_output_ids;
    int item_output_ids_size;
    int item_output_amounts;
    int item_output_amounts_size;
};

struct BuildingsList {
    int buildings_size;
    struct Building *buildings;
};

struct BuildingsList initialise_buildings(FILE * file);
void read_building_data(char *line, struct Building buildings[], const size_t buildings_size,
    struct Province provinces[], struct BuildingType building_types[]);

#endif //WORLD_SIM_BUILDING_H
