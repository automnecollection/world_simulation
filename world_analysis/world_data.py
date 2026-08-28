from dataclasses import dataclass

# EXPERIMENTAL IDEA: Have c and python draw from the data classes so its only written once

@dataclass
class Country:
    tag: str
    name: str

    # IDEA: class Country: data: CountryData class CountryData:
    total_population: dict
    pop_percent_increase: float
    real_total_population: dict
    real_pop_percent_increase: float
    real_accuracy: float

@dataclass
class Province:
    tag: str
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

def parse_list(wrld_file, read_func, **kwargs):
    parsed_objects = []
    file = open(wrld_file, "r")
    for line in file:
        if line == "" or line.__contains__("#"):
            continue
        parsed_object = read_func(line, **kwargs)
        if parsed_object is not None:
            parsed_objects.append(parsed_object)
        else:
            print("ERROR: parsed object is None")
            exit()
    if parsed_objects is not None:
        for obj in parsed_objects:
            if obj is None:
                parsed_objects.remove(obj)
        return parsed_objects
    else:
        print("ERROR: parsed_objects is None")
        return None

def get_list_data_tag(wrld_file, read_data_func, data_list):
    file = open(wrld_file, "r")
    for line in file:
        if line == "" or line.__contains__("#"):
            continue
        for obj in data_list:
            if hasattr(obj, "tag"):
                if line.split("=")[0] == obj.tag:
                    read_data_func(obj, line.split("=")[1])
                else:
                    continue

def read_generic(line: str, **kwargs):
    data_dict = {}
    data_type = kwargs['data_type']
    data = line.split("=")
    tag = data[0]
    data_dict.update({"tag": tag})
    splits = data[1].count(',') + 1
    data_line = data[1]

    i: int = 0
    while i < splits:
        if data_line.__contains__(","):
            token = data_line.split(',')[i]
            if token.__contains__(":"):
                var_token = token.split(":")[0].strip("\n")
                val_token = token.split(":")[1].strip("\n")
                if var_token or val_token is not None:
                    if val_token == "{}":
                        data_dict.update({var_token: {}})
                    else:
                        data_dict.update({var_token: val_token})
                else:
                    continue
        i += 1
    new_obj = data_type(**data_dict)
    return new_obj

# Country initialisation
def read_country_data(country, data):
    data = data.split(",")
    year_start = data[0].split(":")[0]
    population_start = int(data[0].split(":")[1].split(".")[0].strip("\n"))
    year_end = data[1].split(":")[0]
    population_end = int(data[1].split(":")[1].split(".")[0].strip("\n"))

    country.total_population.update({year_start: population_start})
    country.total_population.update({year_end: population_end})

def real_read_country_data(country, data):
    real_year_end = data.split(":")[0]
    real_population_end = int(data.split(":")[1].strip("\n"))
    country.real_total_population.update({real_year_end: real_population_end})

def get_list_obj_from_str(obj_list, var, str_line: str):
    for obj in obj_list:
        if obj is not None:
            if getattr(obj, var) == str_line:
                return obj
        else:
            return None

# Province initialisation
def read_province(province_line: str, countries):
    if province_line == "":
        return None
    data = province_line.split("=")
    tag = data[0]
    owner_country_tag = data[1].split(",")[0].strip("\n")
    country = get_list_obj_from_str(obj_list=countries, var="tag", str_line=owner_country_tag)
    terrain = data[1].split(",")[1].strip("\n")
    climate = data[1].split(",")[2].strip("\n")
    total_population = data[1].split(",")[3].strip("\n")

    province = Province(
        tag=tag,
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
    fields = data[1].split(",")
    province_name = fields[0].strip("\n")
    province = get_list_obj_from_str(obj_list=provinces, var="tag", str_line=province_name)
    size = fields[1].strip("\n")
    culture = fields[2].strip("\n")
    religion = fields[3].strip("\n")

    population = Population(
        province=province,
        size=size,
        culture=culture,
        religion=religion
    )
    return population
