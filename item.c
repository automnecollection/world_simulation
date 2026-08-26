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

    LOOP(population_index, populations_num) {
        struct Population* pop = &populations[population_index];
        float p_size = pop->p_size;
        if (pop->province_id == province_id) {
            // Water - Was * 3.0f
            items[0].demand_amount += p_size * 0.003f;
            // Food
            items[1].demand_amount += p_size * 0.001879f;
            // Refined Oil
            items[3].demand_amount += p_size * 0.0001f;
        }
    }
}

void calc_item_surplus_or_deficit(struct Item items[], int items_num) {
    LOOP(item_index, items_num) {
        struct Item* i = &items[item_index];
        if (i->demand_amount > 0.0f) {
            i->dem_sup_ratio = i->demand_amount / i->supply_amount;
        }
        else {
            i->dem_sup_ratio = 0.0f;
        }
        i->supply_min_demand = i->supply_amount - i->demand_amount;
    }
}

void calc_item_cost(struct Item items[], int items_num, struct NaturalResource nrs[]) {
    LOOP(item_index, items_num) {
        struct Item* i = &items[item_index];
        const struct NaturalResource* r = &nrs[item_index];
        i->cost_ratio = pow(i->dem_sup_ratio, 2.8);
        i->cost = r->base_price * i->cost_ratio;
        i->money_spent_on = i->cost * i->demand_amount;
    }
}
