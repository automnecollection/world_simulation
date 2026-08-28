//
// Created by saiito on 15/08/2026.
//

#include "language.h"

#include "building.h"
#include "country.h"
#include "population.h"
#include "province.h"
#include "item.h"
#include "world_functions.h"


void run_simulation(int sim_days, struct Province provinces[], int provinces_num, struct Country countries[], int countries_num,
                    struct Population populations[], int populations_num, struct BuildingType building_types[], struct NaturalResource *nr_types,
                    struct WorldTime *world_time) {
    world_time->start_year = world_time->year;

    LOOP(province_index, provinces_num) {
        calculate_total_population(&provinces[province_index], populations, populations_num);
        provinces[province_index].start_total_population = provinces[province_index].total_population;
    }
    LOOP(country_index, countries_num) {
        calc_country_total_population(&countries[country_index], provinces, provinces_num);
        countries[country_index].start_total_population = countries[country_index].total_population;
    }

    LOOP(day, sim_days) {
        // printf("day: %d, year: %d\n", world_time.day, world_time.year);
        LOOP(i, populations_num) {
            cmplx_increase_pop_size(&populations[i],
              provinces[populations[i].province_id].urbanisation_rate, provinces[populations[i].province_id].college_education_rate,
              provinces[populations[i].province_id].literacy_rate, provinces[populations[i].province_id].secularism_rate
              );
        }
        LOOP(i, provinces_num) {
            struct Province p = provinces[i];

            calculate_total_population(&provinces[i], populations, populations_num);
            update_item_demand(p.items, populations, populations_num, p.id);
            update_buildings(p.buildings, p.buildings_size, building_types, p.items);
            calc_item_surplus_or_deficit(p.items, p.items_num);
            calc_item_cost(p.items, p.items_num, nr_types);
            calc_levels_needed_for_produced_item_surplus(p.buildings, p.buildings_size, building_types, p.items);
            take_demand_from_item_supplies(p.buildings, p.buildings_size, building_types, p.items);

            if (provinces[i].urbanisation_rate < provinces[i].target_urbanisation_rate) {
                provinces[i].urbanisation_rate += provinces[i].urb_tick * 1.5;
            }
            if (provinces[i].college_education_rate < provinces[i].target_college_education_rate) {
                provinces[i].college_education_rate += provinces[i].col_tick * 1.5;
            }
            if (provinces[i].literacy_rate < provinces[i].target_literacy_rate) {
                provinces[i].literacy_rate += provinces[i].lit_tick * 1.5;
            }
            if (provinces[i].secularism_rate < provinces[i].target_secularism_rate) {
                provinces[i].secularism_rate += provinces[i].sec_tick * 1.5;
            }
        }
        LOOP(country_id, countries_num) {
            calc_country_total_population(&countries[country_id], provinces, provinces_num);
        }
        advance_time(world_time);
    }
}
