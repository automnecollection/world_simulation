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
    float last_supply;
    float levels_for_surplus;
};

struct BuildingsList {
    int buildings_size;
    struct Building *buildings;
};

struct BuildingsList initialise_buildings(FILE * file);

void initialise_building_data(FILE * file, struct Province provinces[], int provinces_size,
    struct BuildingType building_types[], size_t building_types_size);

void read_building_data(char *line, struct Province provinces[], int provinces_size,
    struct BuildingType building_types[], size_t building_types_size);

void update_buildings(struct Building buildings[], const int buildings_num, struct BuildingType building_types[],
    struct Item items[]);

void calc_levels_needed_for_produced_item_surplus(struct Building buildings[], const int buildings_num, struct BuildingType building_types[],
    struct Item items[]);

void take_demand_from_item_supplies(struct Building buildings[], const int buildings_num, struct BuildingType building_types[],
    struct Item items[]);

#endif //WORLD_SIM_BUILDING_H
