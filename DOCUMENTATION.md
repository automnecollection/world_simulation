# World_Sim

## **Data Flow** (main.c)

### **Countries**
- Run the initialise countries functions (initialise_countries)
- Read FILE countries.txt and load country structs and struct size in CountryList
- Return CountryList to main.c

### **Provinces**
- Run the initialise provinces functions (initialise_provinces)
- Read FILE provinces.txt and load province structs and struct size in ProvinceList
- Return ProvinceList to main.c

### **Populations**
- Run the initialise populations functions (initialise_provinces)
- Read FILE provinces.txt and load province structs and struct size in ProvinceList
- Return ProvinceList to main.c

## **Simulation**

### **Weekly Tick**

Calculate the total population of all provinces

#### `population.c`
```c
void calculate_total_population(struct Province *prov, struct Population populations[], int populations_num) {
    float total_population = 0;
    for (int i = 0; i < populations_num; i++) {
        // printf("size: %f", populations[i]->p_size);
        if (populations[i].province_id == prov->id) {
            total_population += populations[i].p_size;
        }
    }
    prov->total_population = total_population;
    prov->total_population_int = (int)total_population;
}
```

### **Monthly Tick**

Calculate the population increase of all population groups

#### `main.c`
```c
const int sim_days = SIM_DAYS;
    for (int i = 0; i < sim_days; i++) {
        for (int j = 0; j < populations_num; j++) {
            if (new_month == true) {
                increase_pop_size(&populations[j], BASE_BIRTH_RATE, month_days);
            }
```

#### `population.c`.
```c
void increase_pop_size(struct Population *pop, const float BASE_BIRTH_RATE) {
    const float new_pop_size = pop->p_size * BASE_BIRTH_RATE;
    pop->p_size = new_pop_size;
    pop->p_size_int = (int)new_pop_size;
}
```