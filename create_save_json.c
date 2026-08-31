#include <stdio.h>

#include "create_save_json.h"

#include "country.h"
#include "province.h"
#include "population.h"
#include "building_type.h"
#include "natural_resource.h"
#include "world_functions.h"

void save_world_json(
    struct Country countries[], int countries_num,
    struct Province provinces[], int provinces_num,
    struct Population populations[], int populations_num,
    struct BuildingType building_types[], int building_types_num,
    const struct WorldTime *world_time) {

    FILE *world_save_file = fopen("../wrld_files/saves/world_save.json", "w");
    if (world_save_file == NULL) {
        perror("Error opening file country result\n");
    }

    fprintf(world_save_file, "{\n");
    push_json(world_save_file, indent, "countries", NULL);
    fprintf(world_save_file, "%s{\n", indent);
    LOOP(c_index, countries_num) {
        push_json(world_save_file, scnd_indent, countries[c_index].tag, NULL);
        fprintf(world_save_file, "%s{\n", scnd_indent);
        push_json(world_save_file, thrd_indent, "name", countries[c_index].name);
        push_json(world_save_file, thrd_indent, "total_population", NULL);
        fprintf(world_save_file, "%s{\n", thrd_indent);
        push_json_num(world_save_file, frth_indent, "1950", countries[c_index].start_total_population);
        fprintf(world_save_file, "%s\"%d\": %f\n", frth_indent, world_time->year, countries[c_index].total_population);
        fprintf(world_save_file, "%s}\n", thrd_indent);
        if (c_index == countries_num - 1) {
            fprintf(world_save_file, "%s}\n", scnd_indent);
        } else {
            fprintf(world_save_file, "%s},\n", scnd_indent);
        }
    }
    fprintf(world_save_file, "%s}\n", indent);
    fprintf(world_save_file, "}\n");
    fclose(world_save_file);
}

void push_json(FILE *json_file, char * indent_num, char *var_str, char *val_str) {
    if (val_str) {
        fprintf(json_file, "%s\"%s\": \"%s\",\n", indent_num, var_str, val_str);
    }
    else
    {
        fprintf(json_file, "%s\"%s\":\n", indent_num, var_str);
    }
}

void push_json_num(FILE *json_file, char * indent_num, char * var_str, float val_str) {
    fprintf(json_file, "%s\"%s\": %f,\n", indent_num, var_str, val_str);
}