#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "building.h"
#include "world.h"
#include "world_functions.h"
#include "province.h"
#include "country.h"
#include "population.h"
#include "natural_resource.h"
#include "building_type.h"
#include "item.h"
#include "create_save.h"

// Get-ChildItem -Recurse -Include *.c,*.h | ForEach-Object { "{0}: {1}" -f $_.Name, (Get-Content $_).Count }

#define DEBUG true
#define DAY 1                                 // Starting day of year (1-365)
#define YEAR 1950                             // Starting year, the game will be designed around starting in 1950
#define SIM_DAYS 27740                        // How many days the simulation will run for, 27740 days = 76 years (runs to 2026 from 1950)
#define BASE_BIRTH_RATE 1.0001                // The base birth rate for dummy population increases
#define LOAD_FROM_RESULTS false
#define FILE_TYPE ".wrld"
#define COUNTRIES_FILE "countries"
#define PROVINCES_FILE "provinces"
#define POPULATIONS_FILE "populations.wrld"

#define DEF_URBANISATION 30
#define DEF_COLLEGE_EDU 3
#define DEF_LITERACY 40
#define DEF_SECULARISM 5

#define DEF_TARGET_URBANISATION 58
#define DEF_TARGET_COLLEGE_EDU 20
#define DEF_TARGET_LITERACY 88
#define DEF_TARGET_SECULARISM 16

int main() {
  const clock_t load_begin = clock();

  struct World world;
  struct WorldTime world_time;

  world_time.day = DAY;
  world_time.year = YEAR;

  printf("Day %d, year %d.\n", world_time.day, world_time.year);

  // Initialise countries
  char countries_file_loc[] = COUNTRIES_FILE;
  if (LOAD_FROM_RESULTS == true) {
    strcat(countries_file_loc, "_result");
  }
  strcat(countries_file_loc, FILE_TYPE);
  if (DEBUG == true) {
    printf("countries_file_loc: %s\n", countries_file_loc);
  }
  FILE *countries_file = fopen(countries_file_loc, "r");

  const struct CountryList country_list = initialise_countries(countries_file);
  struct Country *countries = country_list.countries;
  int countries_num = country_list.countries_num;

  if (DEBUG == true) {
    for (int i = 0; i < countries_num; i++) {
      printf("COUNTRY-%s - ID: %d, NAME: %s\n",
        countries[i].tag, countries[i].id, countries[i].name);
    }
  }

  // Initialise provinces
  char provinces_file_loc[] = PROVINCES_FILE;
  if (LOAD_FROM_RESULTS == true) {
    strcat(provinces_file_loc, "_result");
  }
  strcat(provinces_file_loc, FILE_TYPE);
  if (DEBUG == true) {
    printf("provinces_file_loc: %s\n", provinces_file_loc);
  }
  FILE *provinces_file = fopen(provinces_file_loc, "r");

  const struct ProvinceList province_list = initialise_provinces(provinces_file, countries_file_loc, countries, countries_num);
  struct Province *provinces = province_list.provinces;
  int provinces_num = province_list.provinces_num;

  FILE *provinces_data = fopen("province_data.wrld", "r");

  initialise_temp_province_data(provinces_data, provinces, provinces_num, SIM_DAYS);

  // Initialise populations
  const char * populations_file_loc = POPULATIONS_FILE;
  // if (LOAD_FROM_RESULTS == true) {
  //   populations_file_loc = strcat("result_", populations_file_loc);
  // }
  FILE *populations_file = fopen(populations_file_loc, "r");

  const struct PopulationList populations_list = initialise_populations(populations_file, provinces, provinces_num);
  struct Population *populations = populations_list.populations;
  int populations_num = populations_list.populations_num;

  // Match province and population information
  // for (int i = 0; i < provinces_num; i++) {
  //   for (int j = 0; j < populations_num; j++) {
  //     if (populations[j].province_id == i) {
  //       // printf("Detected province %s.\n", provinces[i].name);
  //       provinces[i].populations[provinces[i].populations_num] = populations[populations_num].id;
  //       provinces[i].populations_num += 1;
  //     }
  //   }
  // }

  if (DEBUG) {
    print_province_data(provinces, provinces_num, populations, populations_num);
  }

  // Initialise building types
  const char * buildings_loc = "buildings.wrld";
  FILE *buildings_file = fopen(buildings_loc, "r");

  const struct BuildingTypesList b_types_list = initialise_building_types(buildings_file);
  struct BuildingType *building_types = b_types_list.building_types;
  int building_types_num = b_types_list.building_types_num;

  // Initialise natural resources
  const char * natural_resources_loc = "natural_resources.wrld";
  FILE *natural_resources_file = fopen(natural_resources_loc, "r");

  const struct NaturalResourcesList nr_list = initialise_nr(natural_resources_file);
  struct NaturalResource *nr_types = nr_list.natural_resources;
  int nr_types_num = nr_list.natural_resources_num;

  if (DEBUG == true) {
    for (int i = 0; i < nr_types_num; i++) {
      printf("NATURAL_RESOURCE-%d - NAME: %s, BASE_PRICE: %f\n",
        nr_types[i].id, nr_types[i].name, nr_types[i].base_price);
    }
  }

  // Initialise items in building types
  for (int i = 0; i < building_types_num; i++) {
    const int type_id = get_item_type(building_types[i].production_type, nr_types, nr_types_num);
    if (DEBUG == true) {
      printf("type_id - %d\n", type_id);
    }
    building_types[i].production_type_id = type_id;
  }

  if (DEBUG == true) {
    for (int i = 0; i < building_types_num; i++) {
      printf("BUILDING_TYPE-%d - NAME: %s, BASE_PRODUCTION: %s (%d) - %d per day.\n",
        building_types[i].id, building_types[i].name, building_types[i].production_type,
        building_types[i].production_type_id, building_types[i].base_production);
    }
  }

  // Initialise items in provinces - assign_items() is from province.c
  assign_items(provinces, provinces_num, nr_types, nr_types_num);

  for (int i = 0; i < provinces_num; i++) {
    if (provinces[i].has_start_data == false) {
      provinces[i].urbanisation_rate = DEF_URBANISATION;
      provinces[i].college_education_rate = DEF_COLLEGE_EDU;
      provinces[i].literacy_rate = DEF_LITERACY;
      provinces[i].secularism_rate = DEF_SECULARISM;
    }
    if (provinces[i].has_target_data == false) {
      provinces[i].target_urbanisation_rate = DEF_TARGET_URBANISATION;
      provinces[i].target_college_education_rate = DEF_TARGET_COLLEGE_EDU;
      provinces[i].target_literacy_rate = DEF_TARGET_LITERACY;
      provinces[i].target_secularism_rate = DEF_TARGET_SECULARISM;

      provinces[i].urb_tick = (provinces[i].target_urbanisation_rate - provinces[i].urbanisation_rate) / SIM_DAYS;
      provinces[i].col_tick = (provinces[i].target_college_education_rate - provinces[i].college_education_rate) / SIM_DAYS;
      provinces[i].lit_tick = (provinces[i].target_literacy_rate - provinces[i].literacy_rate) / SIM_DAYS;
      provinces[i].sec_tick = (provinces[i].target_secularism_rate - provinces[i].secularism_rate) / SIM_DAYS;
    }
  }

  if (DEBUG == true) {
    for (int i = 0; i < provinces_num; i++) {
      printf("PROVINCE %s ITEMS:\n",
          provinces[i].name);
      for (int j = 0; j < provinces[i].items_num; j++) {
        printf("  ITEMS-%d - NAME: %s\n",
          provinces[i].items[j].item_id, provinces[i].items[j].name);
      }
    }
  }

  // Initialise province buildings
  const char * p_buildings_loc = "province_buildings.wrld";
  // strcat(p_buildings_loc, FILE_TYPE);
  FILE *p_buildings_file = fopen(p_buildings_loc, "r");

  assign_buildings(provinces, provinces_num, building_types, building_types_num);
  initialise_building_data(p_buildings_file, provinces, provinces_num, building_types, building_types_num);

  for (int i = 0; i < provinces_num; i++) {
    for (int j = 0; j < building_types_num; j++) {
      if (provinces[i].buildings[j].level > 0) {
        printf("province - %s, name - %s, type_id - %d, level - %d\n",
          provinces[i].name, building_types[provinces[i].buildings[j].id].name,
          provinces[i].buildings[j].id, provinces[i].buildings[j].level);
      }
    }
  }

  printf("\n");

  const clock_t load_end = clock();
  const double load_time_spent = (double)(load_end - load_begin) / CLOCKS_PER_SEC;

  const clock_t sim_begin = clock();

  bool bogos_binted = false;

  // Run simulation
  const int sim_days = SIM_DAYS;
  for (int i = 0; i < sim_days; i++) {
    // printf("day: %d, year: %d\n", world_time.day, world_time.year);
    for (int j = 0; j < populations_num; j++) {
      cmplx_increase_pop_size(&populations[j],
        provinces[populations[j].province_id].urbanisation_rate, provinces[populations[j].province_id].college_education_rate,
        provinces[populations[j].province_id].literacy_rate, provinces[populations[j].province_id].secularism_rate
        );
    }
    for (int j = 0; j < provinces_num; j++) {
      calculate_total_population(&provinces[j], populations, populations_num);
      // for (int k = 0; k < provinces[j].buildings_size; k++) {
      update_item_demand(&provinces[j].items, provinces[j].items_num, populations, populations_num);
      update_buildings(provinces[j].buildings, provinces[j].buildings_size, building_types, &provinces[j].items);

      if (provinces[j].urbanisation_rate < provinces[j].target_urbanisation_rate) {
        provinces[j].urbanisation_rate += provinces[j].urb_tick * 1.5;
      }
      if (provinces[j].id == 30) {
        if ( provinces[j].urbanisation_rate > provinces[j].target_urbanisation_rate ) {
          if ( bogos_binted == false) {
            printf("binted: %d, %d, %s, %f\n", world_time.day, world_time.year, provinces[j].name, provinces[j].urbanisation_rate);
            bogos_binted = true;
          }
        }
      }
      if (provinces[j].college_education_rate < provinces[j].target_college_education_rate) {
        provinces[j].college_education_rate += provinces[j].col_tick * 1.5;
      }
      if (provinces[j].literacy_rate < provinces[j].target_literacy_rate) {
        provinces[j].literacy_rate += provinces[j].lit_tick * 1.5;
      }
      if (provinces[j].secularism_rate < provinces[j].target_secularism_rate) {
        provinces[j].secularism_rate += provinces[j].sec_tick * 1.5;
      }
    }
    advance_time(&world_time);
  }
  printf("\n");

  const clock_t sim_end = clock();
  const double sim_time_spent = (double)(sim_end - sim_begin) / CLOCKS_PER_SEC;

  // Print simulation results
  printf("AFTER %d DAYS\n", sim_days);
  printf("Day %d, year %d.\n", world_time.day, world_time.year);
  print_province_data(provinces, provinces_num, populations, populations_num);

  // Save world data to .wrld file
  const clock_t save_begin = clock();

  save_world(
    countries, countries_num,
    provinces, provinces_num,
    populations, populations_num,
    building_types, building_types_num
    );

  const clock_t save_end = clock();
  const double save_time_spent = (double)(save_end - save_begin) / CLOCKS_PER_SEC;

  // TODO: Save analysis
  // Takes save data and compares it to country data in database
  // Example: simulation goes to 2026, analysis grabs 2026 real life data and compares the two

  // Free data memory
  free_the_nations(countries, countries_num);
  free_provinces(provinces, provinces_num);
  free_the_people(populations, populations_num);

  if (DEBUG == true) {
    printf("load_time_spent: %f\n", load_time_spent);
    printf("simulation_time_spent: %f\n", sim_time_spent);
    printf("save_time_spent: %f\n", save_time_spent);
  }

  getchar();

  return 0;
}
