#pragma once
#include <string.h>
#include <stdio.h>

#ifndef WORLD_SIM_STRUCT_LOADER_H
#define WORLD_SIM_STRUCT_LOADER_H

typedef void (*ParserFunc)(const char *line, int index, void *out_struct, void *ctx);

struct List {
    void *items;
    size_t items_size;
};

struct List initialise_list(FILE *file, size_t struct_size, ParserFunc read_func, void *ctx);

#endif //WORLD_SIM_STRUCT_LOADER_H
