# World_Sim

## **Overview**

### **Data Flow** (main.c)

#### **Countries**
- Run the initialise countries functions (initialise_countries)
- Read FILE countries.txt and load country structs and struct size in CountryList
- Return CountryList to main.c

#### **Provinces**
- Run the initialise provinces functions (initialise_provinces)
- Read FILE provinces.txt and load province structs and struct size in ProvinceList
- Return ProvinceList to main.c

#### **Populations**
- Run the initialise populations functions (initialise_provinces)
- Read FILE provinces.txt and load province structs and struct size in ProvinceList
- Return ProvinceList to main.c

### **Simulation**

#### **Monthly Tick**
Population births will be calculated per population group. Using increase_pop_size() in `population.c`.
```c
void increase_pop_size(struct Population *pop, const float rate) {
    const float new_pop_size = pop->p_size * rate;
    pop->p_size = new_pop_size;
    pop->p_size_int = (int)new_pop_size;
}
```