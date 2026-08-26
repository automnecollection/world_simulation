from dataclasses import dataclass

@dataclass
class Country:
    tag: str
    name: str
    total_population: dict

@dataclass
class Province:
    name: str
    owner_country: str
    terrain: str
    climate: str

@dataclass
class Population:
    size: int
    province: str
    culture: str
    religion: str

def parse_list(wlrd_file: str, read_func, current_dir):
    parsed_objects = []
    file_location = current_dir.parent / "cmake-build-release" / wlrd_file
    file = open(file_location, "r")
    for line in file:
        if line.startswith("#"):
            continue
        parsed_object = read_func(line)
        parsed_objects.append(parsed_object)
    return parsed_objects


def read_country(country_line: str):
    data = country_line.split("=")
    tag = data[0]
    name = data[1].split(",")[0].strip("\n")

    country = Country(tag=tag, name=name, total_population={})
    return country

def init_country_data(wlrd_file: str, current_dir, country_list):
    file_location = current_dir.parent / "cmake-build-release" / wlrd_file
    file = open(file_location, "r")
    for line in file:
        if line.__contains__("#"):
            continue
        for country in country_list:
            if line.split("=")[0] == country.tag:
                read_country_data(country, line.split("=")[1])
    return country_list

def read_country_data(country, data):
    data = data.split(",")
    country.total_population.update({data[0].split(":")[0]: data[0].split(":")[1].strip("\n")})
    country.total_population.update({data[1].split(":")[0]: data[1].split(":")[1].strip("\n")})
