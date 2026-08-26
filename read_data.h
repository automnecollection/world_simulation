#ifndef WORLD_SIM_READ_DATA_H
#define WORLD_SIM_READ_DATA_H

#include "province.h"

struct Population *readdd_population(char *line, int id, struct Province provinces[], int provinces_num);

#endif //WORLD_SIM_READ_DATA_H
