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

## **Initialisation**

Assign items to all provinces (provinces.items)

#### `provinces.c` 

```c
void assign_items_to_provinces(struct Province provinces[], int provinces_num, struct NaturalResource natural_resources[], int nr_types_num) {
    for (int i = 0; i < provinces_num; i++) {
        struct Item *items = calloc(nr_types_num, sizeof(struct Item));
        int items_num = 0;
        for (int j = 0; j < nr_types_num; j++) {
            const struct Item item = {
                .item_id=natural_resources[j].id,
                .name=natural_resources[j].name,
                .demand_amount=0,
                .supply_amount=0,
                .has_deposits=natural_resources[j].has_deposits,
                .deposits_amount=0
            };
            items[items_num] = item;
            items_num++;
        }

        provinces[i].items = items;
        provinces[i].items_num = items_num;
    }
}
```

## **Simulation**

### **Weekly Tick**

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

#### `population.c`

```c
void increase_pop_size(struct Population *pop, const float BASE_BIRTH_RATE, const int month_days) {
    const float new_pop_size = (pop->p_size * BASE_BIRTH_RATE) * month_days;
    pop->p_size = new_pop_size;
    pop->p_size_int = (int)new_pop_size;
}
```

Calculate the total population of all provinces

#### `province.c`

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