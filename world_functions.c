#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "world_functions.h"

void advance_time(struct WorldTime *world_time) {
    world_time->day += 1;
    if (world_time->day > 365) {
        world_time->day = 1;
        world_time->year += 1;
    }
}