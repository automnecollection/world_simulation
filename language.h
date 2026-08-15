#ifndef WORLD_SIM_LANGUAGE_H
#define WORLD_SIM_LANGUAGE_H

#define or ||
#define is ==
#define is_not !=
#define and &&

// General functions
#define LOOP(x, y) for (int x = 0; x < y; x++)      // Loop a set amount of times
#define MATCH(x, y) strcmp(x, y) is 0               // Check if two strings are the same
#define CONTAINS(x, y) strstr(x, y) is_not NULL      // Check if a string contains another string

// Memory
#define STR_ALLOC(x) malloc(strlen(x) + 1)
#define STRUCT_ALLOC(x) malloc(sizeof(struct x))

// Parsing and tokenising
#define TOKENISE(x, y) strtok(x, y)
#define NEXT_TOKEN(x) strtok(NULL, x)
#define STR_INT(x) strtol(x , NULL, 10);
#define STR_FLOAT(x) strtof(x, NULL)
#define EXIT_PROGRAM() getchar(); abort()

#endif //WORLD_SIM_LANGUAGE_H