#ifndef WORLD_SIM_LANGUAGE_H
#define WORLD_SIM_LANGUAGE_H

#define or ||
#define and &&

#define indent      "  "
#define scnd_indent "    "
#define thrd_indent "      "
#define frth_indent "        "

// General functions
#define LOOP(x, y) for (int x = 0; x < y; x++)      // Loop a set amount of times
#define MATCH(x, y) strcmp(x, y) == 0               // Check if two strings are the same
#define CONTAINS(x, y) strstr(x, y) != NULL      // Check if a string contains another string

// Memory
#define STR_ALLOC(x) malloc(strlen(x) + 1)
#define SIZE_ALLOC(x) malloc(sizeof(x))

// Parsing and tokenising
#define TOKENISE(x, y) strtok(x, y)
#define NEXT_TOKEN(x) strtok(NULL, x)
#define STR_INT(x) strtol(x, NULL, 10)
#define STR_FLOAT(x) strtof(x, NULL)
#define EXIT_PROGRAM() getchar(); abort()

#endif //WORLD_SIM_LANGUAGE_H