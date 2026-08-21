#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "list_init.h"

struct List initialise_list(FILE *file, size_t struct_size, ParserFunc read_func, void *ctx) {
    int capacity = 8;
    char line[250];

    struct List list;
    list.items = malloc(capacity * struct_size);
    list.items_size = 0;

    if (file != NULL) {
        while (fgets(line, 100, file)) { if (!strstr(line, "#")) {
                if (list.items_size == capacity) {
                    capacity *= 2;
                    list.items = realloc(
                        list.items,
                        capacity * struct_size);
                }
                void *target_slot = (char*)list.items + (list.items_size * struct_size);
                read_func(line, list.items_size, target_slot, ctx);
                list.items_size += 1;
            }
        }
    }
    else {
        printf("Not able to open the %p file.\n", &file);
    }

    return list;
}
