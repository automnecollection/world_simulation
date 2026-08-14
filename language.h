#ifndef WORLD_SIM_LANGUAGE_H
#define WORLD_SIM_LANGUAGE_H

#define or ||
#define is ==
#define is_not !=

#define tokenise(x) strtok(x, "=")
#define TOKENISE(x) strtok(NULL, x)
#define ALLOC_FOR(x) malloc(strlen(x) + 1)
#define SIZE_ALLOC(x) malloc(sizeof(struct x))
#define get_and_return_null(x, y) if (strstr(x, y) is_not NULL) { return NULL; }
#define float(x) strtof(x, NULL)
#define if_match(x, y) if (strcmp(x, y) is 0)
#define exit() getchar(); abort()


#endif //WORLD_SIM_LANGUAGE_H