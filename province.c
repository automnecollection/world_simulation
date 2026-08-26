#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "language.h"

#include "province.h"
#include "building.h"
#include "building_type.h"
#include "country.h"
#include "population.h"
#include "natural_resource.h"
#include "unit.h"
#include "item.h"

void read_province(const char *line, int index, void *out_struct, void *ctx) {
    struct Province *p = out_struct;
    struct ProvinceParserCtx *data = ctx;

    const char *name_token = strtok(line, "=");
    char *name = STR_ALLOC(name_token);
    strcpy(name, name_token);

    const char *country_tag_token = NEXT_TOKEN(",");
    char *country_tag = STR_ALLOC(country_tag_token);
    strcpy(country_tag, country_tag_token);
    const int owner_country_id = get_country_id_from_tag(country_tag, data->countries, data->countries_num);

    const char *terrain_token = NEXT_TOKEN(",");
    char *terrain = STR_ALLOC(terrain_token);
    strcpy(terrain, terrain_token);

    const char *climate_token = NEXT_TOKEN(",");
    char *climate = STR_ALLOC(climate_token);
    strcpy(climate, climate_token);

    p->id=index;
    p->name=name;
    p->owner_country_id=owner_country_id;
    p->owner_country_tag=country_tag;
    p->terrain=terrain;
    p->climate=climate;
}

void initialise_temp_province_data(FILE * file, struct Province provinces[], int provinces_num, int sim_days) {
    // printf("initialise_temp_province_data");

    char line[200];

    if (file != NULL) {
        while (fgets(line, 100, file)) {
            read_province_data(line, provinces, provinces_num);
        }
    }
    else {
        printf("ERROR ALERT: Failed to load the province data file. Fix it you incompetent mf.");
        getchar();
        abort();
    }

    LOOP(i, provinces_num) {
        provinces[i].urb_tick = (provinces[i].target_urbanisation_rate - provinces[i].urbanisation_rate) / 27740;
        provinces[i].col_tick = (provinces[i].target_college_education_rate - provinces[i].college_education_rate) / 27740;
        provinces[i].lit_tick = (provinces[i].target_literacy_rate - provinces[i].literacy_rate) / 27740;
        provinces[i].sec_tick = (provinces[i].target_secularism_rate - provinces[i].secularism_rate) / 27740;
    }
}

void read_province_data(char *line, struct Province provinces[], int provinces_num) {
    if (CONTAINS(line, "#")) {
        return;
    }

    const char *data_split = TOKENISE(line, ",");
    char *province_name = STR_ALLOC(data_split);
    strcpy(province_name, data_split);
    int province_id = get_province_id_from_name(province_name, provinces, provinces_num);

    const char *type_token = NEXT_TOKEN("=");
    char *type = STR_ALLOC(type_token);
    strcpy(type, type_token);

    const char *str_urbanisation_token = NEXT_TOKEN(",");
    const float urbanisation = STR_FLOAT(str_urbanisation_token);

    const char *str_college_edu_token = NEXT_TOKEN(",");
    const float college_edu = STR_FLOAT(str_college_edu_token);

    const char *str_literacy_token = NEXT_TOKEN(",");
    const float literacy = STR_FLOAT(str_literacy_token);

    const char *str_secularism_token = NEXT_TOKEN(",");
    const float secularism = STR_FLOAT(str_secularism_token);

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
        printf("ERROR ALERT: We didnt get a data type in province %s, it's start or target you stupid bastard!\n",
            provinces[province_id].name);
        EXIT_PROGRAM();
    }

    free(province_name);
    free(type);
}

void print_province_data(struct Province p[], const int provinces_num,
    struct Population populations[], const int populations_num,
    struct BuildingType building_types[], struct Unit units[], const int units_num) {

    LOOP(i, provinces_num) {
        print_province(i, p, populations, populations_num, building_types, units, units_num);
    }
}

void print_province(const int i, struct Province p[],
    struct Population populations[], const int populations_num,
    struct BuildingType building_types[], struct Unit units[], const int units_num) {

    printf("PROVINCE-%d: %s, ", p[i].id, p[i].name);
    printf("COUNTRY OWNER ID: %d, ", p[i].owner_country_id);
    printf("COUNTRY OWNER TAG: %s, ", p[i].owner_country_tag);
    printf("\n");
    printf("    CURRENT GROWTH RATE: %f", p[i].current_growth_rate);
    printf("\n");
    printf("    URBANISATION: %f, COLLEGE_EDU.: %f, LITERACY: %f, SECULARISM: %f\n",
        p[i].urbanisation_rate,
        p[i].college_education_rate,
        p[i].literacy_rate,
        p[i].secularism_rate);
    printf("    CLIMATE: %s, TERRAIN: %s, TOTAL POPULATION: %f\n",
        p[i].climate, p[i].terrain, p[i].total_population);
    print_populations_for_province(populations, populations_num, i);
    printf("    Buildings:\n");
    LOOP(j, p[i].buildings_size) {
        printf("        %s - LEVEL: %d, LAST_OUTPUT: %f, LEVELS NEEDED FOR SURPLUS: %f\n",
            building_types[p[i].buildings[j].id].name, p[i].buildings[j].level, p[i].buildings[j].last_supply,
            p[i].buildings[j].levels_for_surplus);
    }
    printf("    Items:\n");
    LOOP(k, p[i].items_num) {
        printf("        %s - DEMAND: %f, SUPPLY: %f, SUP_B4_DEM: %f\n",
            p[i].items[k].name, p[i].items[k].demand_amount, p[i].items[k].supply_amount,
            p[i].items[k].supply_before_demand);
        printf("                  DEM_SUP_RATIO: %f, COST_MULTIPLIER: %f, COST: %f\n",
            p[i].items[k].dem_sup_ratio, p[i].items[k].cost_ratio, p[i].items[k].cost);
        printf("                  MONEY_SPENT_ON LAST TICK: %f\n",
            p[i].items[k].money_spent_on);
    }
    print_units_for_province(i, units, units_num);
    printf("\n");
}

void assign_items(struct Province provinces[], int provinces_num,
    struct NaturalResource natural_resources[], int nr_types_num) {
    LOOP(i, provinces_num) {
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
            // printf("item name: %s\n", item.name);
        }
        provinces[i].items = items;
        provinces[i].items_num = items_num;
    }
}

int get_province_id_from_name(const char * name, struct Province p[], const int provinces_size) {
    LOOP(province_id, provinces_size) {
        if (strcmp(p[province_id].name, name) == 0) {
            return p[province_id].id;
        }
    }
    return -1;
}

void calc_country_total_population(struct Country *c, struct Province provinces[], const int provinces_num) {
    LOOP(province_id, provinces_num) {
        struct Province* p = &provinces[province_id];
        if (c->id == p->owner_country_id) {
            c->total_population += p->total_population;
        }
    }
}

void free_provinces(struct Province p[], const int provinces_num) {
    LOOP(province_id, provinces_num) {
        free(p[province_id].name);
        free(p[province_id].terrain);
        free(p[province_id].climate);
        free(p[province_id].buildings);
    }
}
