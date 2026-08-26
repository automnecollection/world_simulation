#ifndef WORLD_SIM_CREATE_SAVE_H
#define WORLD_SIM_CREATE_SAVE_H

void save_world(
    struct Country countries[], int countries_num,
    struct Province provinces[], int provinces_num,
    struct Population populations[], int populations_num,
    struct BuildingType building_types[], int building_types_num,
    const struct WorldTime *world_time);

#endif //WORLD_SIM_CREATE_SAVE_H
