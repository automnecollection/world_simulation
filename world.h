//
// A struct for representing the world state
//

#ifndef WORLD_SIM_WORLD_H
#define WORLD_SIM_WORLD_H

struct World {
    struct Country *countries;
    struct Province *provinces;
    struct Population *populations;
    struct BuildingType *buildings;
    struct NaturalResource *natural_resources;
};

#endif //WORLD_SIM_WORLD_H
