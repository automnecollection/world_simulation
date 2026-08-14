#ifndef PROVINCE_H
#define PROVINCE_H

#include "country.h"
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

    bool has_start_data;
    float urbanisation_rate;
    float college_education_rate;
    float literacy_rate;
    float secularism_rate;

    bool has_target_data;
    float target_urbanisation_rate;
    float target_college_education_rate;
    float target_literacy_rate;
    float target_secularism_rate;

    float current_growth_rate;

    float urb_tick;
    float col_tick;
    float lit_tick;
    float sec_tick;

    char populations[];
};

struct ProvinceList {
    int provinces_num;
    struct Province *provinces;
};

// Initialisation
struct ProvinceList initialise_provinces(FILE * file, const char * provinces_file_loc, struct Country countries[], int countries_num);
void *read_province(char *line, int id, struct Country countries[], int countries_num, const char * provinces_file_loc);
void print_province_population(struct Province prov);

void assign_items(struct Province provinces[], int provinces_num,
    struct NaturalResource natural_resources[], int nr_types_num);

void assign_buildings(struct Province provinces[], int provinces_num,
    struct BuildingType building_types[], size_t b_types_size);

// Simulation
void increase_population(struct Province *prov);
struct Province detect_country_provinces(struct Province* provinces[], char *province_owner_tag, char *tag, int provinces_num);
int get_province_id_from_name(const char * name, struct Province provinces[], int provinces_size);
void initialise_temp_province_data(FILE * file, struct Province provinces[], int provinces_num, int sim_days);
void read_province_data(char *line, struct Province provinces[], int provinces_num);
void free_provinces(struct Province provinces[], int provinces_num);
void increase_data_ticks(struct Province prov);

// Sim helpers
void print_province_data(struct Province p[], int provinces_num,
    struct Population populations[], int populations_num,
    struct BuildingType building_types[]);

void print_province_data_province(struct Province p[], int provinces_num,
    struct Population populations[], int populations_num,
    struct BuildingType building_types[]);

int shut_program();

#endif
