#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "natural_resource.h"

struct NaturalResourcesList initialise_nr(FILE * file) {
    int capacity = 8;
    char c_line[200];

    struct NaturalResourcesList nr_list;
    nr_list.natural_resources = malloc(capacity * sizeof *nr_list.natural_resources);

    if (file != NULL) {
        int nr_num = 0;
        while (fgets(c_line, 100, file)) {
            if (nr_num == capacity) {
                capacity *= 2;
                nr_list.natural_resources = realloc(
                    nr_list.natural_resources,
                    capacity * sizeof *nr_list.natural_resources);
            }
            const struct NaturalResource *natural_resource = read_natural_resource(c_line, nr_num);

            if (natural_resource != NULL) {
                nr_list.natural_resources[nr_num] = *natural_resource;
                nr_num += 1;
            }

            free((void*)natural_resource);
        }
        nr_list.natural_resources_num = nr_num;
    }
    else {
        printf("Not able to open the provinces file.\n");
    }

    return nr_list;
}

void *read_natural_resource(char *line, const int id) {
    if (strstr(line, "#") != NULL) {
        return NULL;
    }

    const char *data_split = strtok(line, "=");

    char *name = malloc(strlen(data_split) + 1);
    strcpy(name, data_split);

    char *data = strtok(NULL, "=");

    const char *str_base_price = strtok(data, ",");
    const float base_price = strtof(str_base_price, NULL);

    struct NaturalResource* new_nr = malloc(sizeof(struct NaturalResource));

    if (new_nr != NULL) {
        *new_nr = (struct NaturalResource) {
            .id=id,
            .name=name,
            .base_price=base_price,
            .price=base_price,
            .deposits_amount=0,
            .demand_amount=0,
            .supply_amount=0,
        };
    };

    return new_nr;
}
