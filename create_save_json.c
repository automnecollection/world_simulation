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
    char * indent = "  ";
    char * scnd_indent = "    ";
    char * thrd_indent = "      ";
    char * frth_indent = "        ";

    fprintf(world_save_file, "{\n");
    fprintf(world_save_file, "%s\"countries\":\n", indent);
    fprintf(world_save_file, "%s{\n", indent);
    LOOP(c_index, countries_num) {
        fprintf(world_save_file, "%s\"%s\":\n", scnd_indent, countries[c_index].tag);
        fprintf(world_save_file, "%s{\n", scnd_indent);
        fprintf(world_save_file, "%s\"name\": \"%s\",\n", thrd_indent, countries[c_index].name);
        fprintf(world_save_file, "%s\"total_population\":\n", thrd_indent);
        fprintf(world_save_file, "%s{\n", thrd_indent);
        fprintf(world_save_file, "%s\"1950\": %f,\n", frth_indent, countries[c_index].start_total_population);
        fprintf(world_save_file, "%s\"2026\": %f\n", frth_indent, countries[c_index].total_population);
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
