#include <stdio.h>
#include <stdlib.h>

#include "world_functions.h"
#include "province.h"
#include "country.h"
#include "population.h"

// Get-ChildItem -Recurse -Include *.c,*.h | ForEach-Object { "{0}: {1}" -f $_.Name, (Get-Content $_).Count }

#define DAY 1
#define YEAR 1950
#define SIM_DAYS 1000

int main() {
  struct WorldTime world_time;

  world_time.day = DAY;
  world_time.year = YEAR;

  // Initialise countries
  const char * countries_file_loc = "countries.txt";
  FILE *countries_file = fopen(countries_file_loc, "r");

  const struct CountryList country_list = initialise_countries(countries_file);
  struct Country *countries = country_list.countries;
  int countries_num = country_list.countries_num;

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

  for (int i = 0; i < countries_num; i++) {
    printf("COUNTRY-%s - ID: %d, NAME: %s\n", countries[i].tag, countries[i].id, countries[i].name);
  }

  printf("\n");

  printf("Day %d, year %d.\n", world_time.day, world_time.year);
  for (int i = 0; i < provinces_num; i++) {
    print_province_data(provinces[i]);
    print_populations_for_province(populations, populations_num, i);
  }

  const int sim_days = SIM_DAYS;
  for (int i = 0; i < sim_days; i++) {
    for (int j = 0; j < populations_num; j++) {
      increase_pop_size(&populations[j]);
    }
    // for (int j = 0; j < provinces_num; j++) {
      // update_province(&provinces[j]);
    // }
    advance_time(&world_time);
  }

  printf("\n");

  printf("AFTER %d DAYS\n", sim_days);
  printf("Day %d, year %d.\n", world_time.day, world_time.year);
  for (int i = 0; i < provinces_num; i++) {
    print_province_data(provinces[i]);
    print_populations_for_province(populations, populations_num, i);
  }

  free_countries(countries, countries_num);
  free_provinces(provinces, provinces_num);
  free_populations(populations, populations_num);

  getchar();

  return 0;
}
