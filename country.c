#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "language.h"

#include "country.h"

void read_country(const char* line, const int index, void* out_struct, void *ctx) {
    struct Country *c = out_struct;

    const char *tag_token = strtok(line, "=");
    char *tag = malloc(strlen(tag_token) + 1);
    strcpy(tag, tag_token);

    const char *name_token = NEXT_TOKEN(",");
    char *name = malloc(strlen(name_token) + 1);
    strcpy(name, name_token);

    c->tag = tag;
    c->name = name;
    c->id = (int)index;
}

// TODO: Calculate desired level for each building type in all owned provinces

int get_country_id_from_tag(const char * tag, struct Country countries[], const int countries_size) {
    LOOP(i, countries_size) {
        if (strcmp(countries[i].tag, tag) == 0) {
            return countries[i].id;
        }
    }
    printf("why the fuck not?\n");
    return -1;
}

void free_the_nations(struct Country countries[], const int countries_num) {
    for (int i = 0; i < countries_num; i++) {
        free(countries[i].tag);
        free(countries[i].name);
    }
}
