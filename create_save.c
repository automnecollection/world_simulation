#include <stdio.h>

#include "country.h"
#include "province.h"
#include "population.h"
#include "building_type.h"
#include "natural_resource.h"
#include "world_functions.h"

void save_world(
    struct Country countries[], int countries_num,
    struct Province provinces[], int provinces_num,
    struct Population populations[], int populations_num,
    struct BuildingType building_types[], int building_types_num,
    const struct WorldTime *world_time) {

    FILE *country_result = fopen("countries_result.wrld", "w");
    if (country_result == NULL) {
        perror("Error opening file country result\n");
    }
    LOOP(i, countries_num) {
        fprintf(country_result, "%s=%s\n",
            countries[i].tag, countries[i].name);
    }
    fclose(country_result);

    FILE *countries_data_result = fopen("countries_data_result.wrld", "w");
    LOOP(i, countries_num) {
        fprintf(countries_data_result, "%s=%d:%f,%d:%f\n",
            countries[i].tag,
            world_time->start_year, countries[i].start_total_population,
            world_time->year, countries[i].total_population);
    }
    fclose(countries_data_result);

    FILE *provinces_result = fopen("provinces_result.wrld", "w");
    LOOP(i, provinces_num) {
        fprintf(provinces_result, "%s=%s,%s,%s,%f\n",
            provinces[i].name, provinces[i].owner_country_tag, provinces[i].terrain, provinces[i].climate,
            provinces[i].total_population);
    }
    fclose(provinces_result);

    FILE *populations_result = fopen("populations_result.wrld", "w");
    LOOP(i, populations_num) {
        fprintf(populations_result, "%s=%f,%s,%s\n",
            provinces[populations[i].province_id].name, populations[i].p_size, populations[i].culture, populations[i].religion);
    }
    fclose(populations_result);

    FILE *buildings_result = fopen("buildings_result.wrld", "w");
    LOOP(i, building_types_num) {
        fprintf(buildings_result, "%s=%s,%d,None\n",
            building_types[i].name, building_types[i].production_type, building_types[i].base_production);
    }
    fclose(buildings_result);

}
