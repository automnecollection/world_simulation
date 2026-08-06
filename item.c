#include "item.h"
#include "population.h"

// TODO: Make non hardcoded demand increases, import data from wrld files
void update_item_demand(struct Item items[], struct Population populations[], const int populations_num, const int province_id) {
    for (int i = 0; i < populations_num; i++) {
        if ( populations[i].province_id == province_id) {
            // printf("%f\n", items[0]->demand_amount);
            // Water
            const float water_dem = populations[i].p_size * 3.0f;
            // printf("%s\n", items[0]->name);
            items[0].demand_amount = water_dem;
            // Food
            const float food_dem = populations[i].p_size * 0.001879f;
            // printf("what cunt: %s, %d\n", items[1]->name, items[1]->item_id);
            items[1].demand_amount = food_dem;
        }
    }
}