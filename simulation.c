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

    sim_days = 365;

    LOOP(day, sim_days) {
        // printf("day: %d, year: %d\n", world_time->day, world_time->year);
        /*
            POPULATION TICK DESCRIPTION
             - Calculate increase of every population group based off province statistics like urbanisation, literacy etc.

        */
        LOOP(pop_index, populations_num) {
            struct Population* pop = &populations[pop_index];
            struct Province* p = &provinces[pop->province_id];

            cmplx_increase_pop_size(pop, p->urbanisation_rate, p->college_education_rate, p->literacy_rate, p->secularism_rate);
        }
        /*
            PROVINCE TICK DESCRIPTION

            DAILY TICK:
             - Calculate total population of province from sizes populations located in province.
             - Calculate demand for all items from province populations statistics.
             - Calculate stats related to item demand and supply, including cost and levels required to get an item surplus.
             - Take away the amount of demand from the supply of all items.

             MONTHLY TICK:
             - Building levels will increase according to demand deficit to equalise supply and demand.

            PLANNED TO ADD
            TODO: Stockpiling
             - Populations will buy items, decreasing supply of the items and adding it to a "stockpile of active use" in every province.
             - Some demands / items will be used quickly or instantaneously while stockpiled like water, food and electricity while other goods like computers, furniture, housing will be used for much longer.
             - Demand should decrease while stockpiles are in use, but when stockpiles decrease demand should go back up.
             - There should probably be a variable separate to demand to record population desire / need for items while they are stockpiled and demand is low.

            TODO: Currency
             - If a country has their own currency the government will print it in designated factories.

             TODO: Population wealth
              - Each province population group will earn money from buildings they work in and spend it on demanded items.
              - Population wealth will greatly influence supply and demand as low wealth populations will demand less and demand different items and vice versa for higher wealth pops.
              -

             TODO: GDP
              - The GDP of each country will calculated according to the amount of money spent on each item.

             TODO: Taxation
              - The government will be able to choose to tax privately owned currency in a number of ways.
              - The government will pass laws to choose their taxation policies.
              - For example the government could tax population income.

        */
        LOOP(p_index, provinces_num) {
            struct Province* p = &provinces[p_index];
            struct Country* owner_country = &countries[p->owner_country_id];

            // TODO: I broke the econony system by adding stockpiles. will have to plan out the math before writing code.
            calculate_total_population(p, populations, populations_num);
            update_item_demand(p->items, populations, populations_num, p->id, p->total_population);
            update_buildings(p->buildings, p->buildings_size, building_types, p->items);
            calc_levels_needed_for_produced_item_surplus(p->buildings, p->buildings_size, building_types, p->items);
            calc_item_surplus_or_deficit(p->items, p->items_num);
            calc_item_cost(p->items, p->items_num, nr_types);
            // Stocking piling
            increase_item_stockpiles(p->items, p->items_num);
            decrease_stockpiles(p->items, p->items_num);
            take_demand_from_item_supplies(p, p->buildings, p->buildings_size, building_types, p->items);
            impose_government_taxes(owner_country, p);

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

            if (world_time->day % 30 == 0) {
                update_building_levels(p->buildings, p->buildings_size);
            }
        }
        /*
            COUNTRY TICK DESCRIPTION
             - Calculate total population of country from province total populations.

            TODO: Simple country AI.
             - Countries will determine what buildings / infrastructure they want to build, allocating money.
             - Countries will have relations with other countries. Countries will be able to go to war.
        */
        LOOP(c_index, countries_num) {
            struct Country* c = &countries[c_index];

            calc_country_total_population(c, provinces, provinces_num);
        }
        advance_time(world_time);
    }
}
