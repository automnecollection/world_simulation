#ifndef PROVINCE_H
#define PROVINCE_H

#include "population.h"
#include "natural_resource.h"
#include "item.h"
#include "building_type.h"

struct Province {
    int id;
    char *name;
    int owner_country_id;
    char *owner_country_tag;
    char *terrain;
    char *climate;
    float total_population;
    int total_population_int;
    int populations_num;
    struct Item *items;
    int items_num;
    struct Building *buildings;
    int buildings_size;
    char populations[];
};

struct ProvinceList {
    int provinces_num;
    struct Province *provinces;
};

struct ProvinceList initialise_provinces(FILE * file);

void *read_province(char *line, int id);
void print_province_data(struct Province prov);
void print_province_population(struct Province prov);

void assign_items(struct Province provinces[], int provinces_num,
    struct NaturalResource natural_resources[], int nr_types_num);

void assign_buildings(struct Province provinces[], int provinces_num,
    struct BuildingType building_types[], size_t b_types_size);

void calculate_total_population(struct Province *prov, struct Population populations[], int populations_num);
void update_tick(struct Province *prov, struct Population populations[], int populations_num);
void increase_population(struct Province *prov);
struct Province detect_country_provinces(struct Province* provinces[], char *province_owner_tag, char *tag, int provinces_num);
int get_province_id_from_name(const char * name, struct Province provinces[], int provinces_size);

void free_provinces(struct Province provinces[], int provinces_num, struct ProvinceList *province_list);

#endif
