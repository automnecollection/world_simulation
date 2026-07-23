#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "province.h"

struct ProvinceList initialise_provinces(FILE * file) {
    int capacity = 8;
    char c_line[100];

    struct ProvinceList province_list;
    province_list.provinces = malloc(capacity * sizeof *province_list.provinces);
    province_list.provinces_num = 0;

    if (file != NULL) {
        int provinces_num = 0;
        while (fgets(c_line, 100, file)) {
            if (provinces_num == capacity) {
                capacity *= 2;
                province_list.provinces = realloc(
                    province_list.provinces,
                    capacity * sizeof *province_list.provinces);
            }
            const struct Province *province = read_province(c_line, provinces_num);

            if (province != NULL) {
                province_list.provinces[provinces_num] = *province;
                provinces_num += 1;
            }

            free((void*)province);
        }
        province_list.provinces_num = provinces_num;
    }
    else {
        printf("Not able to open the provinces file.\n");
    }

    return province_list;
}

struct Province *read_province(char *line, const int id) {
    if (strstr(line, "#") != NULL) {
        return NULL;
    }

    const char *data_split = strtok(line, "=");

    char *name = malloc(strlen(data_split) + 1);
    strcpy(name, data_split);

    char *data = strtok(NULL, "=");

    const char *str_owner_country_id = strtok(data, ",");
    const int owner_country_id = strtol(str_owner_country_id, NULL, 10);

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
            .terrain=terrain,
            .climate=climate,
        };
    };

    free(name);
    free(terrain);
    free(climate);

    return new_province;
}

void print_province_data(struct Province const prov) {
    printf("PROVINCE-%d: %s, ", prov.id, prov.name);
    printf("COUNTRY OWNER ID: %d, ", prov.owner_country_id);
    printf("COUNTRY OWNER TAG: %s, ", prov.owner_country_tag);
    printf("TERRAIN: %s, ", prov.terrain);
    printf("CLIMATE: %s\n", prov.climate);

    // printf("pops num %d", prov.populations_num);
    // for (int i = 0; i < prov.populations_num; i++) {
    //     printf("%d - %d", i, prov.populations[i]);
    // }
}

void calculate_total_population(struct Province *prov) {
    prov->total_population = 0;
}

void update_province(struct Province *prov) {
    calculate_total_population(prov);
    // increase_province_populations(prov);
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

void free_provinces(struct Province provinces[], const int provinces_num) {
    for (int i = 0; i < provinces_num; i++) {
        free(provinces[i].name);
        free(provinces[i].terrain);
        free(provinces[i].climate);
    }
}
