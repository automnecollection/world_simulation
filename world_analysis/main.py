from pathlib import Path

import world_data as init
import data_analysis as d_anal


if __name__ == '__main__':
    current_dir = Path(__file__).resolve().parent
    indent = "  "
    scnd_indent = "     "

    countries = init.get_list_data_tag(
        wlrd_file="countries_data_result.wrld",
        read_data_func=init.read_country_data,
        current_dir=current_dir,
        data_list=init.parse_list(
            wlrd_file="countries_result.wrld",
            read_func=init.read_country,
            current_dir=current_dir
        )
    )
    provinces = init.parse_list(
        wlrd_file="provinces_result.wrld",
        read_func=init.read_province,
        current_dir=current_dir,
        countries=countries
    )
    populations = init.parse_list(
        wlrd_file="populations_result.wrld",
        read_func=init.read_population,
        current_dir=current_dir,
        provinces=provinces
    )

    d_anal.data_analysis(
        provinces=provinces
    )

    print("COUNTRIES:")
    for c in countries:
        fifties_pop = c.total_population.get("1950")
        twenties_pop = c.total_population.get("2026")
        print(f"{indent}{c.tag} -- NAME: {c.name}")
        print(f"{scnd_indent}POPULATION 1950: {fifties_pop}, 2026: {twenties_pop}")
    print("PROVINCES:")
    for p in provinces:
        print(f"{indent}{p.name} -- OWNER_COUNTRY: {p.owner_country.name}, TERRAIN: {p.terrain}, CLIMATE: {p.climate}")
        print(f"{scnd_indent}POPULATION 2026: {p.total_population}")
    print("POPULATIONS:")
    for pop in populations:
        print(f"{indent}{pop.province.name} -- SIZE: {pop.size}, CULTURE: {pop.culture}, RELIGION: {pop.religion}")
