#ifndef WORLD_SIM_CREATE_SAVE_JSON_H
#define WORLD_SIM_CREATE_SAVE_JSON_H

#include "country.h"
#include "province.h"
#include "population.h"
#include "building_type.h"
#include "natural_resource.h"
#include "world_functions.h"

void save_world_json(
    struct Country countries[], int countries_num,
    struct Province provinces[], int provinces_num,
    struct Population populations[], int populations_num,
    struct BuildingType building_types[], int building_types_num,
    const struct WorldTime *world_time);

#endif //WORLD_SIM_CREATE_SAVE_JSON_H
