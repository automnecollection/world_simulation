#include <math.h>

#include "config.h"
#include "language.h"

#include "natural_resource.h"
#include "item.h"
#include "population.h"

// TODO: Make non hardcoded demand increases, import data from items wrld files
void update_item_demand(struct Item items[], struct Population populations[], const int populations_num, const int province_id) {
    items[0].demand_amount = 0;
    items[1].demand_amount = 0;

    items[3].demand_amount = 0;

    LOOP(i, populations_num) {
        if (populations[i].province_id == province_id) {
            // Water - Was * 3.0f
            items[0].demand_amount += populations[i].p_size * 0.003f;
            // Food
            items[1].demand_amount += populations[i].p_size * 0.001879f;
            // Refined Oil
            items[3].demand_amount += populations[i].p_size * 0.0001;
        }
    }
}

void calc_item_surplus_or_deficit(struct Item items[], int items_num) {
    LOOP(i, items_num) {
        if (items[i].demand_amount > 0.0f) {
            items[i].dem_sup_ratio = items[i].demand_amount / items[i].supply_amount;
        }
        else {
            items[i].dem_sup_ratio = 0.0f;
        }
        items[i].supply_min_demand = items[i].supply_amount - items[i].demand_amount;
    }
}

void calc_item_cost(struct Item items[], int items_num, struct NaturalResource nrs[]) {
    LOOP(i, items_num) {
        items[i].cost_ratio = pow(items[i].dem_sup_ratio, 2.8);
        items[i].cost = nrs[items[i].item_id].base_price * items[i].cost_ratio;
        items[i].money_spent_on = items[i].cost * items[i].demand_amount;
    }
}
