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
    float supply_before_demand;
    bool has_deposits;
    int deposits_amount;
    float dem_sup_ratio;
    float cost_ratio;
    float supply_min_demand;
};

// Simulation
void update_item_demand(struct Item items[], struct Population populations[], int populations_num, int province_id);
void calc_item_surplus_or_deficit(struct Item items[], int items_num);
void calc_item_cost(struct Item items[], int items_num);

#endif //WORLD_SIM_ITEM_H
