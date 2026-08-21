#ifndef WORLD_SIM_UNIT_H
#define WORLD_SIM_UNIT_H

#include "country.h"
#include "province.h"

struct Unit {
    char * name;
    int index;
    int owner_country_id;
    int province_home_id;

    // Stats
    int manpower;
};

struct UnitParserCtx {
    struct Country *countries;
    int countries_num;
    struct Province *provinces;
    int provinces_num;
};

// Initialisation
void read_unit(const char *line, int index, void *out_struct, void *ctx);

#endif //WORLD_SIM_UNIT_H
