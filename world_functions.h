#ifndef WORLD_SIM_WORLD_FUNCTIONS_H
#define WORLD_SIM_WORLD_FUNCTIONS_H

struct WorldTime {
    int day;
    int year;

    int start_year;
};

void advance_time(struct WorldTime *world_time);

#endif //WORLD_SIM_WORLD_FUNCTIONS_H
