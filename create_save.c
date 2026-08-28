#include <stdio.h>

#include "create_save.h"

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

    FILE *country_result = fopen("../wrld_files/countries_result.wrld", "w");
    if (country_result == NULL) {
        perror("Error opening file country result\n");
    }
    LOOP(i, countries_num) {
        fprintf(country_result, "%s=name:%s,total_population:{},pop_percent_increase:0,real_total_population:{},real_pop_percent_increase:0,real_accuracy:0\n",
            countries[i].tag, countries[i].name);
    }
    fclose(country_result);

    FILE *countries_data_result = fopen("../wrld_files/countries_data_result.wrld", "w");
    LOOP(i, countries_num) {
        fprintf(countries_data_result, "%s=%d:%f,%d:%f\n",
            countries[i].tag,
            world_time->start_year, countries[i].start_total_population,
            world_time->year, countries[i].total_population);
    }
    fclose(countries_data_result);

    FILE *provinces_result = fopen("../wrld_files/provinces_result.wrld", "w");
    LOOP(province_index, provinces_num) {
        fprintf(provinces_result, "%s=%s,%s,%s,%f\n",
            provinces[province_index].name, provinces[province_index].owner_country_tag, provinces[province_index].terrain, provinces[province_index].climate,
            provinces[province_index].total_population);
    }
    fclose(provinces_result);

    FILE *populations_result = fopen("../wrld_files/populations_result.wrld", "w");
    int index = 0;
    LOOP(i, populations_num) {
        fprintf(populations_result, "%d=%s,%f,%s,%s\n",
            index, provinces[populations[i].province_id].name, populations[i].p_size, populations[i].culture, populations[i].religion);
        index++;
    }
    fclose(populations_result);

    FILE *buildings_result = fopen("../wrld_files/buildings_result.wrld", "w");
    LOOP(i, building_types_num) {
        fprintf(buildings_result, "%s=%s,%d,None\n",
            building_types[i].name, building_types[i].production_type, building_types[i].base_production);
    }
    fclose(buildings_result);

}
