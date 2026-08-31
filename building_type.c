#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "building_type.h"
#include "natural_resource.h"
#include "item.h"

struct BuildingTypesList initialise_building_types(FILE * file) {
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
            const struct BuildingType *building = read_building_type(line, b_num);

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

void *read_building_type(char *line, const int id) {
    if (strstr(line, "#") != NULL) {
        return NULL;
    }

    const char *data_split = strtok(line, "=");

    char *name = malloc(strlen(data_split) + 1);
    strcpy(name, data_split);

    char *data = strtok(NULL, "=");

    const char *str_production_type_token = strtok(data, ",");
    // printf("str_production_type_token - %s\n", str_production_type_token);
    char *production_type = malloc(strlen(str_production_type_token) + 1);
    strcpy(production_type, str_production_type_token);

    const char *str_base_production = strtok(NULL, ",");
    printf("str_base_production - %s\n", str_base_production);
    int base_production = strtol(str_base_production, NULL, 10);

    printf("name, production type, base_production - %s, %s, %d\n", name, production_type, base_production);

    struct BuildingType* new_building_type = malloc(sizeof(struct BuildingType));

    if (new_building_type != NULL) {
        *new_building_type = (struct BuildingType) {
            .id=id,
            .name=name,
            .production_type=production_type,
            .production_type_id=-1,
            .base_production=base_production,
        };
    };

    return new_building_type;
}

int get_building_type_id_from_name(const char * name, struct BuildingType building_types[], const size_t building_types_size) {
    for (int i = 0; i < building_types_size; i++) {
        // printf("name: %s, building_types[i].name: %s\n", name, building_types[i].name);
        if (strcmp(building_types[i].name, name) == 0) {
            // printf("got name - name: %s, building_types[i].name: %s, building_types[i].id %d\n", name, building_types[i].name, building_types[i].id);
            return building_types[i].id;
        }
    }
    printf("Could not find building type in for %s", name);
    getchar();
    abort();
}

int get_item_type(const char *type, struct NaturalResource natural_resources[], const int natural_resources_num) {
    for (int i = 0; i < natural_resources_num; i++) {
        // printf("type - %s\n", type);
        // printf("natural_resources[i].name = %s\n", natural_resources[i].name);
        if (strcmp(type, natural_resources[i].name) == 0) {
            // printf("natural_resources[i].id = %d\n", natural_resources[i].id);
            return natural_resources[i].id;
        }
        // printf("did not match - type - %s, natural_resources[i].name - %s\n", type, natural_resources[i].name);
    }
    // printf("could not find item type - %s\n", type);
    return -1;
}
