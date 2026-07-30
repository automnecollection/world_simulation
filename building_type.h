#ifndef WORLD_SIM_BUILDING_TYPE_H
#define WORLD_SIM_BUILDING_TYPE_H

#include "natural_resource.h"

struct BuildingType {
    int id;
    char *name;
    char *production_type;
    int production_type_id;
    int base_production;
};

struct BuildingTypesList {
    int building_types_num;
    struct BuildingType *building_types;
};

struct BuildingTypesList initialise_building_types(FILE * file);
void *read_building_type(char *line, int id);
int get_item_type(const char *type, struct NaturalResource natural_resources[], int natural_resources_num);

#endif //WORLD_SIM_BUILDING_TYPE_H
