#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "population.h"

struct PopulationList initialise_populations(FILE * file) {
    int capacity = 8;
    char c_line[200];

    struct PopulationList population_list;
    population_list.populations = malloc(capacity * sizeof *population_list.populations);
    population_list.populations_num = 0;

    if (file != NULL) {
        int provinces_num = 0;
        while (fgets(c_line, 100, file)) {
            if (provinces_num == capacity) {
                capacity *= 2;
                population_list.populations = realloc(
                    population_list.populations,
                    capacity * sizeof *population_list.populations);
            }
            const struct Population *population = read_population(c_line, provinces_num);

            if (population != NULL) {
                population_list.populations[provinces_num] = *population;
                provinces_num += 1;
            }

            free((void*)population);
        }
        population_list.populations_num = provinces_num;
    }
    else {
        printf("Not able to open the populations file.\n");
    }

    return population_list;
}

struct Population *read_population(char *line, const int id) {
    if (strstr(line, "#") != NULL) {
        return NULL;
    }

    const char *data = strtok(line, ",");
    const int province_id = strtol(data, NULL, 10);

    const char *str_p_size = strtok(NULL, ",");
    const float p_size = strtof(str_p_size, NULL);
    const int p_size_int = (int)p_size;

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

void increase_pop_size(struct Population *pop, const float BASE_BIRTH_RATE) {
    const float new_pop_size = pop->p_size * BASE_BIRTH_RATE;
    pop->p_size = new_pop_size;
    pop->p_size_int = (int)new_pop_size;
}

void cmplx_increase_pop_size(struct Population *pop,
    const float urbanisation, const float college_education, const float literacy, const float secularism) {

    const float growth_rate_factor = calc_growth_factor(urbanisation, college_education, literacy, secularism);
    const float growth_rate = 1.00012f - pow(0.00001 * growth_rate_factor, 1.175);

    const float new_pop_size = pop->p_size * growth_rate;
    pop->p_size = new_pop_size;
    pop->p_size_int = (int)new_pop_size;
}

float calc_growth_factor(const float urb, const float col, const float lit, const float sec) {
    float base =
        (urb * 2.6)
           + (100 * 0.4)
           + (100 * 0.4)
           + (col * 1.55)
           + (lit * 1.6)
           + (sec * 1.1)
           + (100 * 1.1);
    return 0.0137 * pow(base, 1.219);
}

void free_the_people(struct Population populations[], const int populations_num, struct PopulationList *population_list) {
    for (int i = 0; i < populations_num; i++) {
        free(populations[i].culture);
        free(populations[i].religion);
    }
    free(population_list->populations);
    population_list->populations = NULL;
    population_list->populations_num = 0;
}

void print_populations_for_province(struct Population populations[], const int populations_num, const int province_id) {
    printf("  Populations:\n");
    for (int j = 0; j < populations_num; j++) {
        if (populations[j].province_id == province_id) {
            printf("    %s, %s - %f\n", populations[j].culture, populations[j].religion, populations[j].p_size);
        }
    }
    printf("\n");
}
