#ifndef WORLD_SIM_NATURAL_RESOURCES_H
#define WORLD_SIM_NATURAL_RESOURCES_H

#include <stdbool.h>

struct NaturalResource {
    int id;
    char *name;
    bool has_deposits;
    float base_price;
    float price;
    int deposits_amount;
    int demand_amount;
    int supply_amount;
};

struct NaturalResourcesList {
    int natural_resources_num;
    struct NaturalResource *natural_resources;
};

struct NaturalResourcesList initialise_nr(FILE * file);
void *read_natural_resource(char *line, int id);

#endif //WORLD_SIM_NATURAL_RESOURCES_H
