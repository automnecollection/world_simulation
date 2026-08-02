#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "province.h"
#include "building.h"
#include "item.h"

// TODO: Assign buildings broken
void assign_buildings(struct Province provinces[], const int provinces_num, struct BuildingType building_types[], const size_t b_types_size) {
    for (int i = 0; i < provinces_num; i++) {
        struct Building *buildings = calloc(b_types_size, sizeof(struct Building));
        size_t buildings_size = 0;
        for (int j = 0; j < b_types_size; j++) {
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
            read_building_data(line, provinces, &provinces, provinces_size, building_types, building_types_size);
        }
    }
}

// TODO: Remove assign buildings and reduce this function to return a building struct with the required level
void read_building_data(char *line, struct Province provinces[], struct Province *provinces_pointer[], const int provinces_size,
    struct BuildingType building_types[], const size_t building_types_size) {
    if (strstr(line, "#") != NULL) {
        return;
    }

    const char *data_split = strtok(line, "=");

    char *province_name = malloc(strlen(data_split) + 1);
    strcpy(province_name, data_split);
    // printf("provinces[0].name: %s\n", provinces[0].name);
    int province_id = get_province_id_from_name(province_name, provinces, provinces_size);

    char *data = strtok(NULL, "=");

    const char *str_building_type_token = strtok(data, ",");
    // printf("str_production_type_token - %s\n", str_production_type_token);
    char *building_type_name = malloc(strlen(str_building_type_token) + 1);
    strcpy(building_type_name, str_building_type_token);

    int building_type_id = get_building_type_id_from_name(building_type_name, building_types, building_types_size);

    const char *level_token = strtok(NULL, ",");
    const int level = strtol(level_token, NULL, 10);

    // printf("building_type_id : %d\n", building_type_id);

    printf("provinces[province_id].name = %s\n",
        provinces[province_id].name);
    printf("building_types[provinces[province_id].buildings[building_type_id]].name = %s\n",
        building_types[provinces[province_id].buildings[building_type_id].id].name);

    struct Province *prov = &provinces[province_id];
    prov->buildings[building_type_id].level = level;

    // free(data_split);
    // free(province_name);
    // free(building_type_name);
}

void update_buildings(struct Building buildings[], const int buildings_num, struct BuildingType building_types[],
    struct Item *items[]) {
    for (int i = 0; i < buildings_num; i++) {
        if ( buildings[i].level > 0) {
            if ( building_types[buildings[i].id].production_type_id != -1) {
                const int production_amount = building_types[buildings[i].id].base_production * buildings[i].level;
                items[building_types[buildings[i].id].production_type_id]->supply_amount += production_amount;
            }
        }
    }
}
