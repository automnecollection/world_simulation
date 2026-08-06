#pragma once
#include <stdbool.h>

#include "population.h"

#ifndef WORLD_SIM_ITEM_H
#define WORLD_SIM_ITEM_H

struct Item {
    int item_id;
    char * name;
    float demand_amount;
    float supply_amount;
    bool has_deposits;
    int deposits_amount;
};

void update_item_demand(struct Item items[], struct Population populations[], int populations_num, int province_id);

#endif //WORLD_SIM_ITEM_H
