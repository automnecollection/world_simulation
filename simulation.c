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
        /*
            POPULATION TICK DESCRIPTION
            Calculate increase of every population group based off province statistics like urbanisation, literacy etc.

        */
        LOOP(i, populations_num) {
            cmplx_increase_pop_size(&populations[i],
              provinces[populations[i].province_id].urbanisation_rate, provinces[populations[i].province_id].college_education_rate,
              provinces[populations[i].province_id].literacy_rate, provinces[populations[i].province_id].secularism_rate
              );
        }
        /*
            PROVINCE TICK DESCRIPTION
            Calculate total population of province from sizes populations located in province.
            Calculate demand for all items from province populations statistics.
            Calculate stats related to item demand and supply, including cost and levels required to get an item surplus.
            Populations will buy items, decreasing supply of the items and adding it to active use.
            Some demands / items will be used quickly like water, food and electricity while other goods like computers, furniture, housing will be used for a long time.

        */
        LOOP(p_index, provinces_num) {
            struct Province p = provinces[p_index];

            calculate_total_population(&provinces[p_index], populations, populations_num);
            update_item_demand(p.items, populations, populations_num, p.id);
            update_buildings(p.buildings, p.buildings_size, building_types, p.items);
            calc_item_surplus_or_deficit(p.items, p.items_num);
            calc_item_cost(p.items, p.items_num, nr_types);
            calc_levels_needed_for_produced_item_surplus(p.buildings, p.buildings_size, building_types, p.items);
            take_demand_from_item_supplies(p.buildings, p.buildings_size, building_types, p.items);

            if (provinces[p_index].urbanisation_rate < provinces[p_index].target_urbanisation_rate) {
                provinces[p_index].urbanisation_rate += provinces[p_index].urb_tick * 1.5;
            }
            if (provinces[p_index].college_education_rate < provinces[p_index].target_college_education_rate) {
                provinces[p_index].college_education_rate += provinces[p_index].col_tick * 1.5;
            }
            if (provinces[p_index].literacy_rate < provinces[p_index].target_literacy_rate) {
                provinces[p_index].literacy_rate += provinces[p_index].lit_tick * 1.5;
            }
            if (provinces[p_index].secularism_rate < provinces[p_index].target_secularism_rate) {
                provinces[p_index].secularism_rate += provinces[p_index].sec_tick * 1.5;
            }
        }
        /*
            COUNTRY TICK DESCRIPTION
            Calculate total population of country from province total populations.

            TODO: Add simple country AI.
            Countries will determine what buildings / infrastructure they want to build, allocating money.
            Countries will have relations with other countries. Countries will be able to go to war.
        */
        LOOP(country_id, countries_num) {
            calc_country_total_population(&countries[country_id], provinces, provinces_num);
        }
        advance_time(world_time);
    }
}
