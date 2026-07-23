#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "world_functions.h"

void advance_time(struct WorldTime *world_time) {
    int day = world_time->day;
    int year = world_time->year;

    day += 1;
    if (day > 365) {
        day = 1;
        year += 1;
    }

    // printf("day: %d, year: %d,", day, year);

    world_time->day = day;
    world_time->year = year;
}