#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "population.h"

#include "read_data.h"

struct PopulationList initialise_populations(FILE * file, struct Province provinces[], int provinces_size) {
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
            const struct Population *population = read_population(c_line, provinces_num, provinces, provinces_size);

            if (population != NULL) {
                population_list.populations[provinces_num] = *population;
                provinces_num += 1;
            }

            free((void*)population);
        }
        population_list.populations_num = provinces_num;
    }
    else {
        printf("ERROR: Not able to open the populations file.\n");
    }

    return population_list;
}

void increase_pop_size(struct Population *pop, const float BASE_BIRTH_RATE) {
    const float new_pop_size = pop->p_size * BASE_BIRTH_RATE;
    pop->p_size = new_pop_size;
    pop->p_size_int = (int)new_pop_size;
}

void cmplx_increase_pop_size(struct Population *pop,
    const float urbanisation, const float college_education, const float literacy, const float secularism) {

    const float growth_rate_factor = calc_growth_factor(urbanisation, college_education, literacy, secularism);
    const float growth_rate = 1.00012f - pow(0.00001 * growth_rate_factor, 1.155);

    const float new_pop_size = pop->p_size * growth_rate;
    pop->p_size = new_pop_size;
    pop->p_size_int = (int)new_pop_size;

    // TODO: Fix so I can store growth rate in provinces, probably just separate calculating growth rate and increasing pop by growth rate
    // provinces[pop->province_id].current_growth_rate = growth_rate;
}

float calc_growth_factor(const float urb, const float col, const float lit, const float sec) {
    float base =
        (urb * 2.6)
           + (col * 1.53)
           + (lit * 1.6)
           + (sec * 1.3)
           + (100 * 1.9);
    return 0.0137 * pow(base, 1.215);
}

void free_the_people(struct Population populations[], const int populations_num) {
    for (int i = 0; i < populations_num; i++) {
        free(populations[i].culture);
        free(populations[i].religion);
    }
}

void print_populations_for_province(struct Population populations[], const int populations_num, const int province_id) {
    printf("    Populations:\n");
    for (int j = 0; j < populations_num; j++) {
        if (populations[j].province_id == province_id) {
            printf("        %s, %s - %f\n", populations[j].culture, populations[j].religion, populations[j].p_size);
        }
    }
}

void calculate_total_population(struct Province *prov, struct Population populations[], int populations_num) {
    float total_population = 0;
    for (int i = 0; i < populations_num; i++) {
        // printf("size: %f", populations[i]->p_size);
        if (populations[i].province_id == prov->id) {
            total_population += populations[i].p_size;
        }
    }
    prov->total_population = total_population;
    prov->total_population_int = (int)total_population;
}

