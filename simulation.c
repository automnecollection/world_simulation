//
// Created by saiito on 15/08/2026.
//

#include "language.h"

#include "building.h"
#include "population.h"
#include "province.h"
#include "item.h"
#include "world_functions.h"


void run_simulation(int sim_days, struct Province provinces[], int provinces_num,
                    struct Population populations[], int populations_num,
                    struct BuildingType building_types[], struct NaturalResource *nr_types,
                    struct WorldTime world_time) {
    LOOP(i, sim_days) {
        // printf("day: %d, year: %d\n", world_time.day, world_time.year);
        LOOP(j, populations_num) {
            cmplx_increase_pop_size(&populations[j],
              provinces[populations[j].province_id].urbanisation_rate, provinces[populations[j].province_id].college_education_rate,
              provinces[populations[j].province_id].literacy_rate, provinces[populations[j].province_id].secularism_rate
              );
        }
        LOOP(j, provinces_num) {
            struct Province p = provinces[j];

            calculate_total_population(&provinces[j], populations, populations_num);
            update_item_demand(p.items, populations, populations_num, p.id);
            update_buildings(p.buildings, p.buildings_size, building_types, p.items);
            calc_item_surplus_or_deficit(p.items, p.items_num);
            calc_item_cost(p.items, p.items_num, nr_types);
            calc_levels_needed_for_produced_item_surplus(p.buildings, p.buildings_size, building_types, p.items);
            take_demand_from_item_supplies(p.buildings, p.buildings_size, building_types, p.items);

            if (provinces[j].urbanisation_rate < provinces[j].target_urbanisation_rate) {
                provinces[j].urbanisation_rate += provinces[j].urb_tick * 1.5;
            }
            if (provinces[j].college_education_rate < provinces[j].target_college_education_rate) {
                provinces[j].college_education_rate += provinces[j].col_tick * 1.5;
            }
            if (provinces[j].literacy_rate < provinces[j].target_literacy_rate) {
                provinces[j].literacy_rate += provinces[j].lit_tick * 1.5;
            }
            if (provinces[j].secularism_rate < provinces[j].target_secularism_rate) {
                provinces[j].secularism_rate += provinces[j].sec_tick * 1.5;
            }
        }
        advance_time(&world_time);
    }
}
