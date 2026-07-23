#ifndef COUNTRY_H
#define COUNTRY_H

struct Country {
    char * name;
    int id;
    char * tag;
    int provinces_num;

    char provinces[];
};

struct CountryList {
    int countries_num;
    struct Country *countries;
};

struct CountryList initialise_countries(FILE * file);
struct Country read_country(char * line, int id);
void free_countries(struct Country countries[], int countries_num);

#endif
