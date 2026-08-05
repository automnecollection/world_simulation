#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "province.h"

#include "building_type.h"
#include "country.h"
#include "population.h"
#include "natural_resource.h"
#include "item.h"

// TODO: Add ability to list year for population
struct ProvinceList initialise_provinces(FILE * file, const char * provinces_file_loc, struct Country countries[], const int countries_num) {
    int capacity = 8;
    char c_line[200];

    struct ProvinceList province_list;
    province_list.provinces = malloc(capacity * sizeof *province_list.provinces);
    province_list.provinces_num = 0;

    if (file != NULL) {
        int provinces_num = 0;
        while (fgets(c_line, 100, file)) {
            if (strstr(c_line, "YEAR: ")) {
                const char *data_split = strtok(c_line, ":");

            }
            else {
                if (provinces_num == capacity) {
                    capacity *= 2;
                    province_list.provinces = realloc(
                        province_list.provinces,
                        capacity * sizeof *province_list.provinces);
                }
                const struct Province *province = read_province(c_line, provinces_num, countries, countries_num, provinces_file_loc);

                if (province != NULL) {
                    province_list.provinces[provinces_num] = *province;
                    provinces_num += 1;
                }

                free((void*)province);
            }
        }
        province_list.provinces_num = provinces_num;
    }
    else {
        printf("Not able to open the provinces file.\n");
    }

    return province_list;
}

void *read_province(char *line, const int id, struct Country countries[], const int countries_num, const char * provinces_file_loc) {
    if (strstr(line, "#") != NULL) {
        return NULL;
    }

    const char *data_split = strtok(line, "=");

    char *name = malloc(strlen(data_split) + 1);
    strcpy(name, data_split);

    char *data = strtok(NULL, "=");

    const char *country_tag_token = strtok(data, ",");
    char *country_tag = malloc(strlen(data_split) + 1);
    strcpy(country_tag, country_tag_token);
    const int owner_country_id = get_country_id_from_tag(country_tag, countries, countries_num);

    const char *terrain_token = strtok(NULL, ",");
    char *terrain = malloc(strlen(terrain_token) + 1);
    strcpy(terrain, terrain_token);

    const char *climate_token = strtok(NULL, ",");
    char *climate = malloc(strlen(climate_token) + 1);
    strcpy(climate, climate_token);

    struct Province* new_province = malloc(sizeof(struct Province));

    if (new_province != NULL) {
        *new_province = (struct Province) {
            .id=id,
            .name=name,
            .owner_country_id=owner_country_id,
            .owner_country_tag=country_tag,
            .terrain=terrain,
            .climate=climate,
        };
    };

    if (new_province == NULL) {
        free(name);
        free(terrain);
        free(climate);
        printf("ERROR ALERT: Failed to generate the province %d on line %d of the %s file.", id, id+1, provinces_file_loc);
        return NULL;
    }

    return new_province;
}

void initialise_temp_province_data(FILE * file, struct Province provinces[], int provinces_num, int sim_days) {
    // printf("initialise_temp_province_data");

    char line[200];

    if( file != NULL) {
        while (fgets(line, 100, file)) {
            read_province_data(line, provinces, provinces_num);
        }
    }
    else {
        // printf("what?");
    }

    for (int i = 0; i < provinces_num; i++) {
        provinces[i].urb_tick = (provinces[i].target_urbanisation_rate - provinces[i].urbanisation_rate) / sim_days;
        provinces[i].col_tick = (provinces[i].target_college_education_rate - provinces[i].college_education_rate) / sim_days;
        provinces[i].lit_tick = (provinces[i].target_literacy_rate - provinces[i].literacy_rate) / sim_days;
        provinces[i].sec_tick = (provinces[i].target_secularism_rate - provinces[i].secularism_rate) / sim_days;
    }
}

void read_province_data(char *line, struct Province provinces[], int provinces_num) {
    // printf("read_province_data");
    if (strstr(line, "#") != NULL) {
        return;
    }

    const char *data_split = strtok(line, ",");
    char *province_name = malloc(strlen(data_split) + 1);
    strcpy(province_name, data_split);
    // printf("provinces[0].name: %s\n", provinces[0].name);
    int province_id = get_province_id_from_name(province_name, provinces, provinces_num);

    const char *type_token = strtok(NULL, "=");
    char *type = malloc(strlen(type_token) + 1);
    strcpy(type, type_token);

    char *data = strtok(NULL, "=");

    const char *str_urbanisation_token = strtok(data, ",");
    const float urbanisation = strtof(str_urbanisation_token, NULL);

    const char *str_college_edu_token = strtok(NULL, ",");
    const float college_edu = strtof(str_college_edu_token, NULL);

    const char *str_literacy_token = strtok(NULL, ",");
    const float literacy = strtof(str_literacy_token, NULL);

    const char *str_secularism_token = strtok(NULL, ",");
    const float secularism = strtof(str_secularism_token, NULL);

    // printf("type: %s\n", type);

    if (strcmp(type, "start") == 0) {
        provinces[province_id].urbanisation_rate = urbanisation;
        provinces[province_id].college_education_rate = college_edu;
        provinces[province_id].literacy_rate = literacy;
        provinces[province_id].secularism_rate = secularism;
        provinces[province_id].has_start_data = true;
    }
    else if (strcmp(type, "target") == 0) {
        provinces[province_id].target_urbanisation_rate = urbanisation;
        provinces[province_id].target_college_education_rate = college_edu;
        provinces[province_id].target_literacy_rate = literacy;
        provinces[province_id].target_secularism_rate = secularism;
        provinces[province_id].has_target_data = true;
    }
    else {
        printf("ERROR ALERT: We didnt get a target type in province %s, it's start or target you stupid bastard!\n", provinces[province_id].name);
    }

    // printf("province_data %s=%f,%f,%f,%f\n",
    //     provinces[province_id].name,
    //     provinces[province_id].urbanisation_rate,
    //     provinces[province_id].college_education_rate,
    //     provinces[province_id].literacy_rate,
    //     provinces[province_id].secularism_rate);

    free(province_name);
    free(type);
}

void print_province_data(struct Province p[], int provinces_num, struct Population populations[], int populations_num) {
    for (int i = 0; i < provinces_num; i++) {
        printf("PROVINCE-%d: %s, ", p[i].id, p[i].name);
        printf("COUNTRY OWNER ID: %d, ", p[i].owner_country_id);
        printf("COUNTRY OWNER TAG: %s, ", p[i].owner_country_tag);
        printf("\n");
        printf("  CURRENT GROWTH RATE: %f", p[i].current_growth_rate);
        printf("\n");
        printf("  URBANISATION: %f, COLLEGE_EDU.: %f, LITERACY: %f, SECULARISM: %f\n",
            p[i].urbanisation_rate,
            p[i].college_education_rate,
            p[i].literacy_rate,
            p[i].secularism_rate);
        printf("  TERRAIN: %s, ", p[i].terrain);
        printf("CLIMATE: %s, ", p[i].climate);
        printf("TOTAL POPULATION: %f", p[i].total_population);
        printf("\n");
        print_populations_for_province(populations, populations_num, i);
        printf("\n");
    }
    // printf("pops num %d", prov.populations_num);
    // for (int i = 0; i < prov.populations_num; i++) {
    //     printf("%d - %d", i, prov.populations[i]);
    // }
}

// void increase_province_populations(prov) {
//
// }

// struct Province detect_country_provinces(const struct Province* provinces, char *province_owner_tag, char *tag, int provinces_num) {
//     for (int j = 0; j < provinces_num; j++) {
//         if (province_owner_tag == tag) {
//             const struct Province detected_province = provinces[j];
//             return detected_province;
//         }
//     }
// }

void assign_items(struct Province provinces[], int provinces_num,
    struct NaturalResource natural_resources[], int nr_types_num) {
    for (int i = 0; i < provinces_num; i++) {
        struct Item *items = calloc(nr_types_num, sizeof(struct Item));
        int items_num = 0;
        for (int j = 0; j < nr_types_num; j++) {
            const struct Item item = {
                .item_id=natural_resources[j].id,
                .name=natural_resources[j].name,
                .demand_amount=0,
                .supply_amount=0,
                .has_deposits=natural_resources[j].has_deposits,
                .deposits_amount=0
            };
            items[items_num] = item;
            items_num++;
        }

        provinces[i].items = items;
        provinces[i].items_num = items_num;
    }
}

int get_province_id_from_name(const char * name, struct Province provinces[], const int provinces_size) {
    for (int i = 0; i < provinces_size; i++) {
        if (strcmp(provinces[i].name, name) == 0) {
            return provinces[i].id;
        }
    }
    return -1;
}

void free_provinces(struct Province provinces[], const int provinces_num) {
    for (int i = 0; i < provinces_num; i++) {
        free(provinces[i].name);
        free(provinces[i].terrain);
        free(provinces[i].climate);
        free(provinces[i].buildings);
    }
}
