#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "building.h"
#include "natural_resource.h"

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

    const char *str_production_type_token = strtok(data, ",");
    printf("str_production_type_token - %s\n", str_production_type_token);
    char *production_type = malloc(strlen(str_production_type_token) + 1);
    strcpy(production_type, str_production_type_token);

    const char *str_base_production = strtok(NULL, ",");
    printf("str_base_production - %s\n", str_base_production);
    int base_production = strtol(str_base_production, NULL, 10);

    printf("name, base_production - %s, %d\n", name, base_production);

    struct Building* new_building = malloc(sizeof(struct Building));

    if (new_building != NULL) {
        *new_building = (struct Building) {
            .id=id,
            .name=name,
            .production_type=production_type,
            .production_type_id=0,
            .base_production=base_production,
            .item_input_ids_size=0,
            .item_input_amounts_size=0,
            .item_output_ids_size=0,
            .item_output_amounts_size=0
        };
    };

    return new_building;
}

int get_item_type(const char *type, struct NaturalResource natural_resources[], const int natural_resources_num) {
    for (int i = 0; i < natural_resources_num; i++) {
        printf("type - %s\n", type);
        printf("natural_resources[i].name = %s\n", natural_resources[i].name);
        if (strcmp(type, natural_resources[i].name) == 0) {
            printf("natural_resources[i].id = %d\n", natural_resources[i].id);
            return natural_resources[i].id;
        }
        printf("did not match - type - %s, natural_resources[i].name - %s\n", type, natural_resources[i].name);
    }
    printf("could not find item type - %s\n", type);
    return -1;
}

void update_buildings(struct Building buildings[], const int buildings_num) {

}
