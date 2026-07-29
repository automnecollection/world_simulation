#ifndef WORLD_SIM_BUILDING_H
#define WORLD_SIM_BUILDING_H

#include "natural_resource.h"

struct Building {
    int id;
    char *name;
    char *production_type;
    int production_type_id;
    int base_production;

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

struct BuildingTypesList {
    int building_types_num;
    struct Building *building_types;
};

struct BuildingTypesList initialise_buildings(FILE * file);
void *read_building(char *line, int id);
int get_item_type(const char *type, struct NaturalResource natural_resources[], int natural_resources_num);

#endif //WORLD_SIM_BUILDING_H
