#include <stdio.h>

#include "country.h"
#include "province.h"
#include "population.h"
#include "building_type.h"
#include "natural_resource.h"

void save_world(
    struct Country countries[], int countries_num,
    struct Province provinces[], int provinces_num,
    struct Population populations[], int populations_num,
    struct BuildingType building_types[], int building_types_num,
    struct NaturalResource nr_types[], int nr_types_num) {

    FILE *country_result = fopen("countries_result.txt", "w");
    for (int i = 0; i < countries_num; i++) {
        fprintf(country_result, "%s=%s,None\n",
            countries[i].name, countries[i].tag);
    }

    FILE *provinces_result = fopen("provinces_result.txt", "w");
    for (int i = 0; i < provinces_num; i++) {
        fprintf(provinces_result, "%s=%d,%s,%s,None\n",
            provinces[i].name, provinces[i].owner_country_id, provinces[i].terrain, provinces[i].climate);
    }

    FILE *populations_result = fopen("populations_result.txt", "w");
    for (int i = 0; i < populations_num; i++) {
        fprintf(populations_result, "%d,%d,%s,%s,None\n",
            populations[i].province_id, populations[i].p_size_int, populations[i].culture, populations[i].religion);
    }

    FILE *buildings_result = fopen("buildings_result.txt", "w");
    for (int i = 0; i < building_types_num; i++) {
        fprintf(buildings_result, "%s=%s,%d,None\n",
            building_types[i].name, building_types[i].production_type, building_types[i].base_production);
    }

}
