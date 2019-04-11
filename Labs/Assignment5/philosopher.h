#ifndef PHILOSOPHER_H_
#define PHILOSOPHER_H_

#include <semaphore.h>

#include "table.h"

typedef struct philosopher
{
  int id;
  table_t* table;
  sem_t* left;
  sem_t* right;
  int rand_state;
} philosopher_t;

philosopher_t*
make_philosopher (int id, sem_t* left, sem_t* right, table_t* table, int seed);

void
phil_run (void*);

#endif
