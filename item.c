#include <math.h>

#include "config.h"
#include "language.h"

#include "item.h"
#include "population.h"

// TODO: Make non hardcoded demand increases, import data from items wrld files
void update_item_demand(struct Item items[], struct Population populations[], const int populations_num, const int province_id) {
    items[0].demand_amount = 0;
    items[1].demand_amount = 0;
    for (int i = 0; i < populations_num; i++) {
        if ( populations[i].province_id is province_id) {
            // Water - Was * 3.0f
            items[0].demand_amount += populations[i].p_size * 0.003f;
            // Food
            items[1].demand_amount += populations[i].p_size * 0.001879f;
        }
    }
}

void calc_item_surplus_or_deficit(struct Item items[], int items_num) {
    for (int i = 0; i < items_num; i++) {
        if ( items[i].demand_amount && items[i].supply_amount > 0.0f) {
            items[i].dem_sup_ratio = items[i].demand_amount / items[i].supply_amount;
            printf("items[i].demand_amount: %f\n", items[i].demand_amount);
            printf("items[i].supply_amount: %f\n", items[i].supply_amount);
            items[i].supply_min_demand = items[i].supply_amount - items[i].demand_amount;
            printf("items[i].supply_min_demand: %f\n", items[i].supply_min_demand);
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
