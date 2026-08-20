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

    // printf("building_type_id : %d\n", building_type_id);

    // printf("provinces[province_id].name = %s\n",
    //     provinces[province_id].name);
    // printf("building_types[provinces[province_id].buildings[building_type_id]].name = %s\n",
    //     building_types[provinces[province_id].buildings[building_type_id].id].name);

    struct Province *prov = &provinces[province_id];
    prov->buildings[building_type_id].level = level;

    // free(data_split);
    // free(province_name);
    // free(building_type_name);
}

void update_buildings(struct Building buildings[], const int buildings_num, struct BuildingType building_types[],
    struct Item items[]) {
    LOOP(i, buildings_num) {
        if ( buildings[i].level > 0) { if ( building_types[buildings[i].id].production_type_id != -1) {
                const float production_amount = (float)building_types[buildings[i].id].base_production * (float)buildings[i].level;
                items[building_types[buildings[i].id].production_type_id].supply_amount += production_amount;
                buildings[i].last_supply = production_amount;
            }
        }
    }
}

void calc_levels_needed_for_produced_item_surplus(struct Building buildings[], const int buildings_num, struct BuildingType building_types[],
    struct Item items[]) {
    LOOP(i, buildings_num) {
        const struct BuildingType* building_type = &building_types[buildings[i].id];
        if (buildings[i].level > 0) { if (building_type->production_type_id != -1) {
                const struct Item* item = &items[building_type->production_type_id];
                if (item->supply_amount > item->demand_amount) {
                    buildings[i].levels_for_surplus = 0.0f;
                }
                else {
                    buildings[i].levels_for_surplus = fabsf(item->supply_min_demand)/building_type->base_production;
                }
            }
        }
    }
}

void take_demand_from_item_supplies(struct Building buildings[], const int buildings_num, struct BuildingType building_types[],
    struct Item items[]) {
    LOOP(i, buildings_num) {
        const struct BuildingType* building_type = &building_types[buildings[i].id];
        if (buildings[i].level > 0) { if (building_type->production_type_id != -1) {
                struct Item* item = &items[building_type->production_type_id];
                item->supply_before_demand = item->supply_amount;
                item->supply_amount -= item->demand_amount;
                if (item->supply_amount < 0.0f) {
                    item->supply_amount = 0.0f;
                }
            }
        }
    }
}
