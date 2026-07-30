#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "country.h"

struct CountryList initialise_countries(FILE *file) {
    int capacity = 8;
    char c_line[200];

    struct CountryList country_list;
    country_list.countries = malloc(capacity * sizeof *country_list.countries);
    country_list.countries_num = 0;

    if (file != NULL) {
        int countries_num = 0;
        while (fgets(c_line, 100, file)) {
            if (countries_num == capacity) {
                capacity *= 2;
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
    const char *data_split = strtok(line, "=");

    char *name = malloc(strlen(data_split) + 1);
    strcpy(name, data_split);

    const char *tag_token = strtok(NULL, ",");
    char *tag = malloc(strlen(tag_token) + 1);
    strcpy(tag, tag_token);

    const struct Country new_country = {
        name, id, tag
      };

    return new_country;
}

void free_countries(struct Country countries[], const int countries_num, struct CountryList *country_list) {
    for (int i = 0; i < countries_num; i++) {
        free(countries[i].name);
        free(countries[i].tag);
    }
}
