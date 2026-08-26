#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "country.h"
#include "province.h"
#include "unit.h"

void read_unit(const char *line, int index, void *out_struct, void *ctx) {
    struct Unit *u = out_struct;
    struct UnitParserCtx *data = ctx;

    const char *country_tag_token = strtok(line, "=");
    char *country_tag = malloc(strlen(country_tag_token) + 1);
    strcpy(country_tag, country_tag_token);
    const int owner_country_id = get_country_id_from_tag(country_tag, data->countries, data->countries_num);

    char *province_name_token = NEXT_TOKEN(",");
    char *province_name = malloc(strlen(province_name_token) + 1);
    strcpy(province_name, province_name_token);
    int province_id = get_province_id_from_name(province_name, data->provinces, data->provinces_num);

    const char *str_manpower = NEXT_TOKEN(",");
    const int manpower = STR_INT(str_manpower);

    u->index = index;
    u->owner_country_id = owner_country_id;
    u->province_home_id = province_id;
    u->manpower = manpower;
}

void print_units_for_province(const int province_id, struct Unit u[], const int units_num) {
    printf("    Units:\n");
    LOOP(unit_index, units_num) {
        if (u[unit_index].province_home_id == province_id) {
            printf("        UNIT-%d - %d, %d\n",
                u[unit_index].province_home_id, u[unit_index].owner_country_id, u[unit_index].manpower);
        }
    }
}
