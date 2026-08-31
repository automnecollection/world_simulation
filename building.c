#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "province.h"
#include "building.h"
#include "item.h"

// TODO: Assign buildings broken
void assign_buildings(struct Province provinces[], const int provinces_num, struct BuildingType building_types[], const size_t b_types_size) {
    LOOP(i, provinces_num) {
        struct Building *buildings = calloc(b_types_size, sizeof(struct Building));
        size_t buildings_size = 0;
        LOOP(j, b_types_size) {
            const struct Building building = {
                .id=building_types[j].id,
                .level=0,
                .item_input_ids=0,
                .item_input_ids_size=0,
                .item_input_amounts=0,
                .item_input_amounts_size=0,
                .item_output_ids=0,
                .item_output_ids_size=0,
                .item_output_amounts=0,
                .item_output_amounts_size=0
            };
            buildings[buildings_size] = building;
            buildings_size++;
        }

        provinces[i].buildings = buildings;
        provinces[i].buildings_size = buildings_size;
    }
}

void initialise_building_data(FILE * file, struct Province provinces[], int provinces_size,
    struct BuildingType building_types[], const size_t building_types_size) {
    char line[200];

    if (file != NULL) {
        while (fgets(line, 100, file)) {
            read_building_data(line, provinces, provinces_size, building_types, building_types_size);
        }
    }
}

// TODO: Remove assign buildings and reduce this function to return a building struct with the required level
void read_building_data(char *line, struct Province provinces[], const int provinces_size,
    struct BuildingType building_types[], const size_t building_types_size) {
    if (CONTAINS(line, "#")) {
        return;
    }

    const char *data_split = strtok(line, "=");

    char *province_name = malloc(strlen(data_split) + 1);
    strcpy(province_name, data_split);
    // printf("provinces[0].name: %s\n", provinces[0].name);
    int province_id = get_province_id_from_name(province_name, provinces, provinces_size);

    char *data = strtok(NULL, "=");

    const char *building_type_token = strtok(data, ",");
    // printf("str_production_type_token - %s\n", str_production_type_token);
    char *building_type_name = STR_ALLOC(building_type_token);
    strcpy(building_type_name, building_type_token);
    int building_type_id = get_building_type_id_from_name(building_type_name, building_types, building_types_size);

    const char *level_token = NEXT_TOKEN(",");
    const int level = STR_INT(level_token);

    struct Building *b = &provinces[province_id].buildings[building_type_id];
    b->level = level;
}

void update_buildings(struct Building buildings[], const int b_num, struct BuildingType building_types[], struct Item items[]) {
    LOOP(b_index, b_num) {
        if (buildings[b_index].level > 0) {
            if (building_types[buildings[b_index].id].production_type_id != -1) {
                const float production_amount = (float)building_types[buildings[b_index].id].base_production * (float)buildings[b_index].level;
                if (buildings[b_index].id == 1)
                {
                    items[building_types[buildings[b_index].id].production_type_id].supply_amount = production_amount;
                    buildings[b_index].last_supply = production_amount;
                }
                else
                {
                    items[building_types[buildings[b_index].id].production_type_id].supply_amount += production_amount;
                    buildings[b_index].last_supply = production_amount;
                }
            }
        }
    }
}

void calc_levels_needed_for_produced_item_surplus(struct Building buildings[], const int b_num, struct BuildingType b_types[], struct Item items[]) {
    LOOP(b_index, b_num) {
        struct Building* b = &buildings[b_index];
        const struct BuildingType* b_type = &b_types[b->id];
        if (b->level > 0) {
            if (b_type->production_type_id != -1) {
                const struct Item* i = &items[b_type->production_type_id];
                if (i->supply_amount > i->demand_amount) {
                    b->levels_for_surplus = 0.0f;
                }
                else {
                    float stockpiles_div_prod = i->stockpile_amount / (float)b_type->base_production;
                    if (stockpiles_div_prod < 0.0) {
                        stockpiles_div_prod = 0.0f;
                    }
                    b->levels_for_surplus = (fabsf(i->supply_min_demand)/(float)b_type->base_production) - stockpiles_div_prod;
                    if (b->levels_for_surplus < 0.0f) {
                        b->levels_for_surplus = 0.0f;
                    }
                }
            }
        }
    }
}

void update_building_levels(struct Building buildings[], const int b_num) {
    LOOP(b_index, b_num) {
        struct Building* b = &buildings[b_index];
        b->level += b->levels_for_surplus;
    }
}

void take_demand_from_item_supplies(struct Province* p, struct Building buildings[], const int b_num, struct BuildingType b_types[], struct Item items[]) {
    LOOP(b_index, b_num) {
        const struct BuildingType* b_type = &b_types[b_index];

        if (buildings[b_index].level > 0) {
            if (b_type->production_type_id != -1) {
                struct Item* i = &items[b_type->production_type_id];

                i->demand_amount -= i->stockpile_amount;
                if (i->demand_amount < 0.0f) {
                    i->demand_amount = 0.0f;
                }
                i->supply_before_demand = i->supply_amount;
                if (i->supply_before_demand < 0.0f) {
                    i->supply_before_demand = 0.0f;
                }
                i->supply_amount -= i->demand_amount;
                if (i->supply_amount < 0.0f) {
                    i->supply_amount = 0.0f;
                }
                p->private_currency_ownership += (i->supply_before_demand - i->supply_amount) * i->cost;
            }
        }
    }
}
