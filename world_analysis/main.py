from pathlib import Path

import world_data as wd


if __name__ == '__main__':
    current_dir = Path(__file__).resolve().parent

    # 1950s
    nf_countries = wd.parse_list("countries.wrld", wd.read_country, current_dir)
    # 2020s
    tt_countries = wd.init_country_data(
        "countries_data_result.wrld", current_dir,
        wd.parse_list(
            "countries_result.wrld", wd.read_country, current_dir
        ),
    )

    countries = zip(nf_countries, tt_countries)
    for nf, tt in countries:
        print(nf, tt)
