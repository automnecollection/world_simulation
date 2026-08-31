#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "population.h"
#include "province.h"

void read_population(const char* line, const int index, void* out_struct, void *ctx) {
    struct Population *p = out_struct;
    struct PopulationParserCtx *data = ctx;

    const char *data_split = strtok(line, "=");
    char *province_name = malloc(strlen(data_split) + 1);
    strcpy(province_name, data_split);
    int province_id = get_province_id_from_name(province_name, data->provinces, data->provinces_num);

    const char *str_p_size = strtok(NULL, ",");
    const float p_size = strtof(str_p_size, NULL);
    const int p_size_int = (int)p_size;

    const char *culture_token = strtok(NULL, ",");
    char *culture = malloc(strlen(culture_token) + 1);
    strcpy(culture, culture_token);

    const char *religion_token = strtok(NULL, ",");
    char *religion = malloc(strlen(religion_token) + 1);
    strcpy(religion, religion_token);

    p->id=index;
    p->province_id=province_id;
    p->p_size=p_size;
    p->p_size_int=p_size_int;
    p->culture=culture;
    p->religion=religion;
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

    pop->p_size = pop->p_size * growth_rate;

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

// This is deep af
void free_the_people(struct Population populations[], const int populations_num) {
    LOOP(population_index, populations_num) {
        const struct Population* pop = &populations[population_index];
        free(pop->culture);
        free(pop->religion);
    }
}

void print_populations_for_province(struct Population populations[], const int populations_num, const int province_id) {
    printf("    Populations:\n");
    LOOP(population_index, populations_num) {
        const struct Population* pop = &populations[population_index];
        if (pop->province_id == province_id) {
            printf("        %s, %s - %f\n", pop->culture, pop->religion, pop->p_size);
        }
    }
}

void calculate_total_population(struct Province *prov, struct Population populations[], int populations_num) {
    float total_population = 0;
    LOOP(population_index, populations_num) {
        const struct Population* pop = &populations[population_index];
        if (pop->province_id == prov->id) {
            total_population += pop->p_size;
        }
    }
    prov->total_population = total_population;
    prov->total_population_int = (int)total_population;
}
