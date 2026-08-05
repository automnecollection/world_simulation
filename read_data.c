#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "province.h"
#include "read_data.h"

struct Population *read_population(char *line, const int id, struct Province provinces[], int provinces_num) {
    if (strstr(line, "#") != NULL) {
        return NULL;
    }

    const char *data_split = strtok(line, "=");
    char *province_name = malloc(strlen(data_split) + 1);
    strcpy(province_name, data_split);
    int province_id = get_province_id_from_name(province_name, provinces, provinces_num);

    const char *str_p_size = strtok(NULL, ",");
    const float p_size = strtof(str_p_size, NULL);
    const int p_size_int = (int)p_size;

    printf("p_size: %f\n", p_size);

    const char *culture_token = strtok(NULL, ",");
    char *culture = malloc(strlen(culture_token) + 1);
    strcpy(culture, culture_token);

    const char *religion_token = strtok(NULL, ",");
    char *religion = malloc(strlen(religion_token) + 1);
    strcpy(religion, religion_token);

    struct Population* new_population = malloc(sizeof(struct Population));

    if (new_population != NULL) {
        *new_population = (struct Population) {
            .id=id,
            .province_id=province_id,
            .p_size=p_size,
            .p_size_int=p_size_int,
            .culture=culture,
            .religion=religion
        };
    }

    return new_population;
}
