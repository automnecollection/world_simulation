from pathlib import Path

import world_data as wd


if __name__ == '__main__':
    current_dir = Path(__file__).resolve().parent

    countries = wd.init_country_data(
        "countries_data_result.wrld", current_dir,
        wd.parse_list(
            "countries_result.wrld", wd.read_country, current_dir
        ),
    )

    for c in countries:
        print(c)
