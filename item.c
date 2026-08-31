#include <math.h>

#include "config.h"
#include "language.h"

#include "natural_resource.h"
#include "item.h"
#include "population.h"

// TODO: Make non hardcoded demand increases, import data from items wrld files
void update_item_demand(struct Item items[], struct Population populations[], const int populations_num, const int province_id, float province_total_population) {
    items[0].demand_amount = 0;
    items[1].demand_amount = 0;

    items[3].demand_amount = 0;

    // Housing
    items[5].demand_amount = 0;

    LOOP(population_index, populations_num) {
        struct Population* pop = &populations[population_index];
        float p_size = pop->p_size;
        if (pop->province_id == province_id) {
            // Water - Was * 3.0f
            items[0].demand_amount += (p_size * 0.003f) * (p_size / province_total_population)- items[0].stockpile_amount;
            // Food
            items[1].demand_amount += (p_size * 0.001879f) * (p_size / province_total_population)- items[1].stockpile_amount;
            // Refined Oil
            items[3].demand_amount += (p_size * 0.0001f) * (p_size / province_total_population)- items[3].stockpile_amount;
            // Housing
            items[5].demand_amount += (p_size * 0.001f) * (p_size / province_total_population)- items[5].stockpile_amount;
        }
    }
}

void calc_item_surplus_or_deficit(struct Item items[], int items_num) {
    LOOP(item_index, items_num) {
        struct Item* i = &items[item_index];

        float dem_min_stockpiles = i->demand_amount - i->stockpile_amount;
        if (dem_min_stockpiles < 0.0f) {
            dem_min_stockpiles = 0.0f;
        }
        if (i->demand_amount > 0.0f) {
            i->dem_sup_ratio = dem_min_stockpiles / i->supply_amount;
            if (i->dem_sup_ratio < 0.0f) {
                i->dem_sup_ratio = 0.0f;
            }
        }
        else {
            i->dem_sup_ratio = 0.0f;
        }
        i->supply_min_demand = i->supply_amount - dem_min_stockpiles;
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

void increase_item_stockpiles(struct Item items[], int items_num) {
    LOOP(item_index, items_num) {
        struct Item* i = &items[item_index];

        float dem_min_sup = i->demand_amount - i->supply_amount;
        if (dem_min_sup < 0.0f) {
            dem_min_sup = 0.0f;
        }
        i->stockpile_amount += i->demand_amount - dem_min_sup;
        if (i->stockpile_amount < 0.0f) {
            i->stockpile_amount = 0.0f;
        }
    }
}

// TODO: Make non hardcoded stockpile decreases, import data from items wrld files
void decrease_stockpiles(struct Item items[], int items_num) {
    // Water - Was * 3.0f
    items[0].stockpile_amount *= 0.1f;
    // Food
    items[1].stockpile_amount *= 0.05f;
    // Refined Oil
    items[3].stockpile_amount *= 0.3f;
    // Housing
    items[5].stockpile_amount *= 0.9999f;
}
