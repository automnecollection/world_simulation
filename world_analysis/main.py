from pathlib import Path

import world_data as init
import data_analysis as d_anal


if __name__ == '__main__':
    current_dir = Path(__file__).resolve().parent
    indent = "  "
    scnd_indent = "     "

    countries = init.parse_list(
        wrld_file=current_dir.parent / "wrld_files" / "countries_result.wrld",
        read_func=init.read_generic,
        data_type=init.Country,
    )
    init.get_list_data_tag(
        wrld_file=current_dir.parent / "wrld_files" / "countries_data_result.wrld",
        read_data_func=init.read_country_data,
        data_list=countries
    )
    init.get_list_data_tag(
        wrld_file="real_countries_data.wrld",
        read_data_func=init.real_read_country_data,
        data_list=countries,
    )
    provinces = init.parse_list(
        wrld_file=current_dir.parent / "wrld_files" / "provinces_result.wrld",
        read_func=init.read_province,
        countries=countries
    )
    populations = init.parse_list(
        wrld_file=current_dir.parent / "wrld_files" / "populations_result.wrld",
        read_func=init.read_population,
        provinces=provinces
    )

    d_anal.data_analysis(
        countries=countries
    )

    average_real_accuracy = 0.0
    c_num = 0
    for c in countries:
        if float(c.real_accuracy) > 0.0:
            average_real_accuracy += c.real_accuracy
            c_num += 1
    average_real_accuracy: float = average_real_accuracy / c_num

    print("COUNTRIES:")
    for c in countries:
        fifties_pop = c.total_population.get("1950")
        twenties_pop = c.total_population.get("2026")
        real_twenties_pop = c.real_total_population.get("2026")
        print(f"{indent}{c.tag} -- NAME: {c.name}")
        print(f"{scnd_indent}POPULATION 1950: {fifties_pop}, 2026: {twenties_pop}, PERCENTAGE_CHANGE: %{c.pop_percent_increase}")
        print(f"{scnd_indent}REAL POPULATION IN 2026: {real_twenties_pop}, ACCURACY: {c.real_accuracy}")
    print(f"AVERAGE ACCURACY: {average_real_accuracy}")

    print("PROVINCES:")
    for p in provinces:
        if p is not None:
            print(f"{indent}{p.tag} -- OWNER_COUNTRY: {p.owner_country.name}, TERRAIN: {p.terrain}, CLIMATE: {p.climate}")
            print(f"{scnd_indent}POPULATION 2026: {p.total_population}")
    print("POPULATIONS:")
    for pop in populations:
        if pop is not None:
            print(f"{indent}-- SIZE: {pop.size}, CULTURE: {pop.culture}, RELIGION: {pop.religion}")
