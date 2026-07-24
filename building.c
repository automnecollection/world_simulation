#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "building.h"

struct BuildingTypesList initialise_buildings(FILE * file) {
    int capacity = 8;
    char line[200];

    struct BuildingTypesList b_types_list;
    b_types_list.building_types = malloc(capacity * sizeof *b_types_list.building_types);
    b_types_list.building_types_num = 0;

    if (file != NULL) {
        int b_num = 0;
        while (fgets(line, 100, file)) {
            if (b_num == capacity) {
                capacity *= 2;
                b_types_list.building_types = realloc(
                    b_types_list.building_types,
                    capacity * sizeof *b_types_list.building_types);
            }
            const struct Building *building = read_building(line, b_num);

            if (building != NULL) {
                b_types_list.building_types[b_num] = *building;
                b_num += 1;
            }

            free((void*)building);
        }
        b_types_list.building_types_num = b_num;
    }
    else {
        printf("Not able to open the building_types file.\n");
    }

    return b_types_list;
}

void *read_building(char *line, const int id) {
    if (strstr(line, "#") != NULL) {
        return NULL;
    }

    const char *data_split = strtok(line, "=");

    char *name = malloc(strlen(data_split) + 1);
    strcpy(name, data_split);

    char *data = strtok(NULL, "=");

    const char *str_base_production = strtok(data, ",");
    const int base_production = strtol(str_base_production, NULL, 10);

    struct Building* new_building = malloc(sizeof(struct Building));

    if (new_building != NULL) {
        *new_building = (struct Building) {
            .id=id,
            .name=name,
            .base_production=base_production,
            .item_input_ids_size=0,
            .item_input_amounts_size=0,
            .item_output_ids_size=0,
            .item_output_amounts_size=0
        };
    };

    return new_building;
}
