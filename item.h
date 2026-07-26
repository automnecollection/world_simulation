#pragma once
#include <stdbool.h>

#ifndef WORLD_SIM_ITEM_H
#define WORLD_SIM_ITEM_H

struct Item {
    int item_id;
    char * name;
    int demand_amount;
    int supply_amount;
    bool has_deposits;
    int deposits_amount;
};

#endif //WORLD_SIM_ITEM_H
