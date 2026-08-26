from dataclasses import dataclass

# EXPERIMENTAL IDEA: Have c and python draw from the data classes so its only written once

@dataclass
class Country:
    tag: str
    name: str
    total_population: dict

@dataclass
class Province:
    name: str
    owner_country: Country
    terrain: str
    climate: str
    total_population: str

@dataclass
class Population:
    province: Province
    size: str
    culture: str
    religion: str

def parse_list(wlrd_file: str, read_func, current_dir, **kwargs):
    parsed_objects = []
    file_location = current_dir.parent / "cmake-build-release" / wlrd_file
    file = open(file_location, "r")
    for line in file:
        if line == "" or line.__contains__("#"):
            continue
        parsed_object = read_func(line, **kwargs)
        parsed_objects.append(parsed_object)
    return parsed_objects

def get_list_data_tag(wlrd_file: str, read_data_func, current_dir, data_list):
    file_location = current_dir.parent / "cmake-build-release" / wlrd_file
    file = open(file_location, "r")
    for line in file:
        if line == "" or line.__contains__("#"):
            continue
        for obj in data_list:
            if line.split("=")[0] == obj.tag:
                read_data_func(obj, line.split("=")[1])
    return data_list

def read_generic(line: str, **kwargs):
    data = line.split("=")

# Country initialisation
def read_country(country_line: str, **kwargs):
    data = country_line.split("=")
    tag = data[0]
    name = data[1].split(",")[0].strip("\n")

    country = Country(
        tag=tag,
        name=name,
        total_population={}
    )
    return country

def read_country_data(country, data):
    data = data.split(",")
    year_start = data[0].split(":")[0]
    population_start = data[0].split(":")[1].strip("\n")
    year_end = data[1].split(":")[0]
    population_end = data[1].split(":")[1].strip("\n")

    country.total_population.update({year_start: population_start})
    country.total_population.update({year_end: population_end})

def get_obj_from_var(var: str, var_type, objs):
    for o in objs:
        if getattr(o, var_type) == var:
            return o

# Province initialisation
def read_province(province_line: str, countries):
    if province_line == "":
        return None
    data = province_line.split("=")
    name = data[0]
    country = get_obj_from_var(data[1].split(",")[0].strip("\n"), "tag", countries)
    terrain = data[1].split(",")[1].strip("\n")
    climate = data[1].split(",")[2].strip("\n")
    total_population = data[1].split(",")[3].strip("\n")

    province = Province(
        name=name,
        owner_country=country,
        terrain=terrain,
        climate=climate,
        total_population=total_population
    )
    return province

# Population initialisation
def read_population(population_line: str, provinces):
    if population_line == "":
        return None
    data = population_line.split("=")
    province = get_obj_from_var(data[0].strip("\n"), "name", provinces)
    size = data[1].split(",")[0].strip("\n")
    culture = data[1].split(",")[1].strip("\n")
    religion = data[1].split(",")[2].strip("\n")

    population = Population(
        province=province,
        size=size,
        culture=culture,
        religion=religion
    )
    return population
