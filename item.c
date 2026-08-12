#include <math.h>

#include "item.h"
#include "population.h"

// TODO: Make non hardcoded demand increases, import data from items wrld files
void update_item_demand(struct Item items[], struct Population populations[], const int populations_num, const int province_id) {
    for (int i = 0; i < populations_num; i++) {
        if ( populations[i].province_id == province_id) {
            // Water
            const float water_dem = populations[i].p_size * 3.0f;
            items[0].demand_amount = water_dem;
            // Food
            const float food_dem = populations[i].p_size * 0.001879f;
            items[1].demand_amount = food_dem;
        }
    }
}

void calc_item_surplus_or_deficit(struct Item items[], int items_num) {
    for (int i = 0; i < items_num; i++) {
        if ( items[i].demand_amount || items[i].supply_amount == 0.0) {
            items[i].dem_sup_ratio = items[i].demand_amount / items[i].supply_amount;
            items[i].supply_min_demand = items[i].supply_amount - items[i].demand_amount;
        }
        else {
            items[i].dem_sup_ratio = 0.0f;
        }
    }
}

void calc_item_cost(struct Item items[], int items_num) {
    for (int i = 0; i < items_num; i++) {
        items[i].cost_ratio = pow(items[i].dem_sup_ratio, 2.8);
    }
}
