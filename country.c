#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "language.h"

#include "country.h"

struct CountryList initialise_countries(FILE *file) {
    int capacity = 8;
    char c_line[250];

    struct CountryList country_list;
    country_list.countries = malloc(capacity * sizeof *country_list.countries);
    country_list.countries_num = 0;

    if (file != NULL) {
        int countries_num = 0;
        while (fgets(c_line, 100, file)) {
            if (countries_num == capacity) {
                capacity *= 4;
                country_list.countries = realloc(
                    country_list.countries,
                    capacity * sizeof *country_list.countries);
            }
            const struct Country country = read_country(c_line, countries_num);
            country_list.countries[countries_num] = country;
            countries_num += 1;
        }
        country_list.countries_num = countries_num;
    }
    else {
        printf("Not able to open the countries file.\n");
    }

    return country_list;
}

struct Country read_country(char * line, int id) {
    const char *tag_token = strtok(line, "=");
    char *tag = STR_ALLOC(tag_token);
    strcpy(tag, tag_token);

    const char *name_token = NEXT_TOKEN(",");
    char *name = STR_ALLOC(name_token);
    strcpy(name, name_token);

    const struct Country new_country = {
        name, id, tag
      };

    return new_country;
}

// TODO: Calculate desired level for each building type in all owned provinces

int get_country_id_from_tag(const char * tag, struct Country countries[], const int countries_size) {
    LOOP(i, countries_size) {
        if (MATCH(countries[i].tag, tag)) {
            return countries[i].id;
        }
    }
    return -1;
}

void free_the_nations(struct Country countries[], const int countries_num) {
    for (int i = 0; i < countries_num; i++) {
        free(countries[i].name);
        free(countries[i].tag);
    }
}
