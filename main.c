#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "world.h"
#include "world_functions.h"
#include "province.h"
#include "country.h"
#include "population.h"
#include "natural_resource.h"
#include "building.h"
#include "item.h"

// Get-ChildItem -Recurse -Include *.c,*.h | ForEach-Object { "{0}: {1}" -f $_.Name, (Get-Content $_).Count }

#define DAY 1
#define YEAR 1950
// 18250 = 50 years
// 27740 = 76 years (2026)
#define SIM_DAYS 27740 // 50 years
#define BASE_BIRTH_RATE 1.000041

int main() {
  struct World world;
  struct WorldTime world_time;

  world_time.day = DAY;
  world_time.year = YEAR;

  // Initialise countries
  const char * countries_file_loc = "countries.txt";
  FILE *countries_file = fopen(countries_file_loc, "r");

  const struct CountryList country_list = initialise_countries(countries_file);
  struct Country *countries = country_list.countries;
  int countries_num = country_list.countries_num;

  for (int i = 0; i < countries_num; i++) {
    printf("COUNTRY-%s - ID: %d, NAME: %s\n", countries[i].tag, countries[i].id, countries[i].name);
  }

  // Initialise provinces
  const char * provinces_file_loc = "provinces.txt";
  FILE *provinces_file = fopen(provinces_file_loc, "r");

  const struct ProvinceList province_list = initialise_provinces(provinces_file);
  struct Province *provinces = province_list.provinces;
  int provinces_num = province_list.provinces_num;

  // Match country and province information
  for (int j = 0; j < countries_num; j++) {
    for (int i = 0; i < provinces_num; i++) {
      if (countries[j].id == provinces[i].owner_country_id) {
        countries[j].provinces[countries[j].provinces_num] = provinces[i].id;
        countries[j].provinces_num += 1;

        provinces[i].owner_country_tag = countries[j].tag;
      }
    }
  }

  printf("\n");

  // Initialise populations
  const char * populations_file_loc = "populations.txt";
  FILE *populations_file = fopen(populations_file_loc, "r");

  const struct PopulationList populations_list = initialise_populations(populations_file);
  struct Population *populations = populations_list.populations;
  int populations_num = populations_list.populations_num;

  // Match province and population information
  for (int i = 0; i < provinces_num; i++) {
    if (populations[populations_num].province_id == i) {
      // printf("Detected province %s.\n", provinces[i].name);
      provinces[i].populations[provinces[i].populations_num] = populations[populations_num].id;
      provinces[i].populations_num += 1;
    }
  }

  for (int i = 0; i < provinces_num; i++) {
    print_province_data(provinces[i]);
    print_populations_for_province(populations, populations_num, i);
  }

  // Initialise buildings
  const char * buildings_loc = "buildings.txt";
  FILE *buildings_file = fopen(buildings_loc, "r");

  const struct BuildingTypesList b_types_list = initialise_buildings(buildings_file);
  struct Building *building_types = b_types_list.building_types;
  int building_types_num = b_types_list.building_types_num;

  for (int i = 0; i < building_types_num; i++) {
    printf("BUILDING_TYPE-%d - NAME: %s, BASE_PRODUCTION: %d\n",
      building_types[i].id, building_types[i].name, building_types[i].base_production);
  }

  // Initialise natural resources (nr)
  const char * natural_resources_loc = "natural_resources.txt";
  FILE *natural_resources_file = fopen(natural_resources_loc, "r");

  const struct NaturalResourcesList nr_list = initialise_nr(natural_resources_file);
  struct NaturalResource *nr_types = nr_list.natural_resources;
  int nr_types_num = nr_list.natural_resources_num;

  for (int i = 0; i < nr_types_num; i++) {
    printf("NATURAL_RESOURCE-%d - NAME: %s, BASE_PRICE: %f\n",
      nr_types[i].id, nr_types[i].name, nr_types[i].base_price);
  }

  assign_items_to_provinces(provinces, provinces_num, nr_types, nr_types_num);

  for (int i = 0; i < provinces_num; i++) {
    printf("PROVINCE %s ITEMS:\n",
        provinces[i].name);
    for (int j = 0; j < provinces[i].items_num; j++) {
      printf("  ITEMS-%d - NAME: %s\n",
        provinces[i].items[j].item_id, provinces[i].items[j].name);
    }
  }

  printf("\n");

  printf("Day %d, year %d.\n", world_time.day, world_time.year);

  clock_t begin = clock();

  const int sim_days = SIM_DAYS;
  for (int i = 0; i < sim_days; i++) {
    for (int j = 0; j < populations_num; j++) {
      increase_pop_size(&populations[j], BASE_BIRTH_RATE);
    }
    for (int j = 0; j < provinces_num; j++) {
      update_province(&provinces[j], populations, populations_num);
    }
    advance_time(&world_time);
  }

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("\n");

  printf("AFTER %d DAYS\n", sim_days);
  printf("Day %d, year %d.\n", world_time.day, world_time.year);
  for (int i = 0; i < provinces_num; i++) {
    print_province_data(provinces[i]);
    print_populations_for_province(populations, populations_num, i);
  }

  free_countries(countries, countries_num, &country_list);
  free_provinces(provinces, provinces_num, &province_list);
  free_populations(populations, populations_num, &populations_list);

  getchar();

  printf("simulation time_spent: %f", time_spent);

  return 0;
}
