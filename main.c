#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "config.h"
#include "language.h"

#include "simulation.h"
#include "list_init.h"
#include "create_save.h"

#include "building.h"
#include "world.h"
#include "world_functions.h"
#include "province.h"
#include "country.h"
#include "population.h"
#include "natural_resource.h"
#include "building_type.h"
#include "item.h"
#include "unit.h"

// Get-ChildItem -Recurse -Include *.c,*.h | ForEach-Object { "{0}: {1}" -f $_.Name, (Get-Content $_).Count }

int main() {
  const clock_t load_begin = clock();

  struct World world;
  struct WorldTime world_time;

  world_time.day = DAY;
  world_time.year = YEAR;

  printf("Day %d, year %d.\n", world_time.day, world_time.year);

  // Initialise countries
  char countries_file_loc[64];
  strcpy(countries_file_loc, COUNTRIES_FILE);
  if (LOAD_FROM_RESULTS == true) {
    strcat(countries_file_loc, "_result");
  }
  strcat(countries_file_loc, FILE_TYPE);
  FILE *countries_file = fopen(countries_file_loc, "r");

  struct List countries_list = initialise_list(countries_file, sizeof(struct Country), read_country, NULL);
  struct Country *countries = countries_list.items;
  size_t countries_num = countries_list.items_size;

  // Initialise provinces
  char provinces_file_loc[64];
  strcpy(provinces_file_loc,PROVINCES_FILE);
  if (LOAD_FROM_RESULTS == true) {
    strcat(provinces_file_loc, "_result");
  }
  strcat(provinces_file_loc, FILE_TYPE);
  FILE *provinces_file = fopen(provinces_file_loc, "r");

  // const struct ProvinceList province_list = initialise_provinces(provinces_file, countries_file_loc, countries, countries_num);
  struct ProvinceParserCtx province_ctx = {countries, countries_num};
  printf("check\n");
  struct List province_list = initialise_list(provinces_file, sizeof(struct Province), read_province, &province_ctx);
  printf("check2\n");
  struct Province *provinces = province_list.items;
  int provinces_num = province_list.items_size;

  // LOOP(i, provinces_num) {
  //   printf("%s\n", provinces[i].name);
  // }

  FILE *provinces_data = fopen("province_data.wrld", "r");

  initialise_temp_province_data(provinces_data, provinces, provinces_num, SIM_DAYS);

  printf("checken\n");

  // Initialise populations
  const char * populations_file_loc = POPULATIONS_FILE;
  FILE *populations_file = fopen(populations_file_loc, "r");

  struct PopulationParserCtx population_ctx = {provinces, provinces_num};
  const struct List populations_list = initialise_list(populations_file, sizeof(struct Population), read_population, &population_ctx);
  struct Population *populations = populations_list.items;
  int populations_num = populations_list.items_size;

  printf("check3\n");

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

  // Initialise items in building types
  for (int i = 0; i < building_types_num; i++) {
    const int type_id = get_item_type(building_types[i].production_type, nr_types, nr_types_num);
    // if (DEBUG == true) {
      printf("type_id - %d\n", type_id);
    // }
    building_types[i].production_type_id = type_id;
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
    }

    provinces[i].urb_tick = (provinces[i].target_urbanisation_rate - provinces[i].urbanisation_rate) / 27740;
    provinces[i].col_tick = (provinces[i].target_college_education_rate - provinces[i].college_education_rate) / 27740;
    provinces[i].lit_tick = (provinces[i].target_literacy_rate - provinces[i].literacy_rate) / 27740;
    provinces[i].sec_tick = (provinces[i].target_secularism_rate - provinces[i].secularism_rate) / 27740;
  }

  // Initialise province buildings
  const char * p_buildings_loc = "province_buildings.wrld";
  // strcat(p_buildings_loc, FILE_TYPE);
  FILE *p_buildings_file = fopen(p_buildings_loc, "r");

  assign_buildings(provinces, provinces_num, building_types, building_types_num);
  initialise_building_data(p_buildings_file, provinces, provinces_num, building_types, building_types_num);

  LOOP(i, provinces_num) {
    LOOP(j, building_types_num) {
      if (provinces[i].buildings[j].level > 0) {
        printf("province - %s, name - %s, type_id - %d, level - %d\n",
          provinces[i].name, building_types[provinces[i].buildings[j].id].name,
          provinces[i].buildings[j].id, provinces[i].buildings[j].level);
      }
    }
  }

  // Initialise units
  const char * units_loc = "units.wrld";
  FILE *units_file = fopen(units_loc, "r");

  struct UnitParserCtx unit_ctx = {countries, countries_num, provinces, provinces_num};
  struct List units_list = initialise_list(units_file, sizeof(struct Unit), read_unit, &unit_ctx);
  struct Unit *units = units_list.items;
  size_t units_num = units_list.items_size;

  const clock_t load_end = clock();
  const double load_time_spent = (double)(load_end - load_begin) / CLOCKS_PER_SEC;

  if (DEBUG) {
    printf("countries_file_loc: %s\n", countries_file_loc);

    for (int i = 0; i < countries_num; i++) {
      printf("COUNTRY-%s - ID: %d, NAME: %s\n",
        countries[i].tag, countries[i].id, countries[i].name);
    }

    printf("provinces_file_loc: %s\n", provinces_file_loc);

    for (int i = 0; i < nr_types_num; i++) {
      printf("NATURAL_RESOURCE-%d - NAME: %s, BASE_PRICE: %f\n",
        nr_types[i].id, nr_types[i].name, nr_types[i].base_price);
    }

    for (int i = 0; i < building_types_num; i++) {
      printf("BUILDING_TYPE-%d - NAME: %s, BASE_PRODUCTION: %s (%d) - %d per day.\n",
        building_types[i].id, building_types[i].name, building_types[i].production_type,
        building_types[i].production_type_id, building_types[i].base_production);
    }

    LOOP(i, provinces_num) {
      printf("PROVINCE %s ITEMS:\n",
          provinces[i].name);
      LOOP(j, provinces[i].items_num) {
        printf("  ITEMS-%d - NAME: %s, DEMAND: %f, SUPPLY: %f\n",
          provinces[i].items[j].item_id, provinces[i].items[j].name, provinces[i].items[j].demand_amount, provinces[i].items[j].supply_amount);
      }
    }
  }

  const clock_t sim_begin = clock();

  // Run simulation
  const int sim_days = SIM_DAYS;

  run_simulation(
    sim_days,
    provinces, provinces_num, countries, countries_num, populations, populations_num, building_types, nr_types,
    &world_time
    );

  const clock_t sim_end = clock();
  const double sim_time_spent = (double)(sim_end - sim_begin) / CLOCKS_PER_SEC;

  // Print simulation results
  printf("AFTER %d DAYS\n", sim_days);
  printf("Day %d, year %d.\n", world_time.day, world_time.year);
  // print_province_data(provinces, provinces_num, populations, populations_num, building_types, units, units_num);
  print_province(0, provinces, populations, populations_num, building_types, units, units_num);

  // Save world data to .wrld file
  const clock_t save_begin = clock();

  save_world(
      countries, countries_num, provinces, provinces_num, populations, populations_num, building_types, building_types_num
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

  printf("load_time_spent: %f\n", load_time_spent);
  printf("simulation_time_spent: %f\n", sim_time_spent);
  printf("save_time_spent: %f\n", save_time_spent);

  getchar();

  return 0;
}
