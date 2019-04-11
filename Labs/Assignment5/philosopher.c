#include "philosopher.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

philosopher_t*
make_philosopher (int id, sem_t* left, sem_t* right, table_t* t, int seed)
{
  philosopher_t* p = (philosopher_t*)malloc (sizeof (philosopher_t));
  p->id = id;
  p->rand_state = seed * (id) + 12345;
  p->table = t;
  p->left = left;
  p->right = right;
  return p;
}

int
rand_between (int low, int high, int* state)
{
  return low + rand_r (state) % (high - low);
}

static
void
phil_sleep (philosopher_t* p)
{
  DEBUG_PRINT("%d is now sleeping...\n", p->id);
  usleep (rand_between (1, 10, &p->rand_state) * SLEEP_FACTOR);
  DEBUG_PRINT("%d woke up!\n", p->id);
}

static
void
phil_eat (philosopher_t* p)
{
  DEBUG_PRINT("%d is going to try to eat\n", p->id);
  table_checkin (p->table, p->id, p);
  DEBUG_PRINT("%d is eating\n", p->id);
  usleep (rand_between (1, 10, &p->rand_state) * EAT_FACTOR);
  DEBUG_PRINT("%d is done eating!\n", p->id);
  table_checkout (p->table, p->id, p);
  DEBUG_PRINT("%d has returned their utensils\n", p->id);
}

void
phil_run (void* p_)
{
  philosopher_t* p = (philosopher_t*)p_;
  usleep (rand_between (1, 10, &p->rand_state) * ARRIVE_FACTOR);
  DEBUG_PRINT("%d has arrived at the table\n", p->id);
  while (p->table->open)
  {
    phil_eat (p);
    if (p->table->open)
      phil_sleep (p);
  }
  DEBUG_PRINT("%d has left the table\n", p->id);
}
