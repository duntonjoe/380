#include <stdlib.h>
#include <pthread.h>

#include "table.h"
#include "philosopher.h"

table_t*
make_table (int places)
{
  // WKK -- BEGIN DO NOT CHANGE
  table_t* t = (table_t*)malloc (sizeof (table_t));
  t->places = places;
  t->open = true;
  t->valid = true;
  t->forks = (sem_t*)malloc (sizeof (sem_t) * (places + 1));
  t->access = (int*)malloc (sizeof (int) * places);
  for (int i = 0; i < places; ++i)
  {
    sem_init (t->forks + i, 0, 1);
    t->access[i] = 0;
  }
  sem_init (t->forks + places, 0, 1);
  // WKK -- END DO NOT CHANGE

  // TODO -- add whatever else you need to initialize
  pthread_mutex_init(&t->mutex, NULL);

  return t;
}

void
free_table (table_t* t)
{
  free (t->access);
  free (t->forks);
  free (t);
}

void
table_checkin (table_t* t, int id, philosopher_t* p)
{
  // TODO -- have p aquire its chopsticks safely
  
  
  // WKK -- BEGIN DO NOT CHANGE
  sem_wait(&t->forks[t->places]);
  int leftID = p->left - t->forks;
  int rightID = p->right - t->forks;
  if (++t->access[leftID] != 1) {
    t->valid = false;
  }
  if (++t->access[rightID] != 1) {
    t->valid = false;
  }
  sem_post(&t->forks[t->places]);
  // WKK -- END DO NOT CHANGE

}

void
table_checkout (table_t* t, int id, philosopher_t* p)
{
  // TODO -- have p release its chopsticks safely


  // WKK -- BEGIN DO NOT CHANGE
  sem_wait(&t->forks[t->places]);
  int leftID = p->left - t->forks;
  int rightID = p->right - t->forks;
  --t->access[leftID];
  --t->access[rightID];
  sem_post(&t->forks[t->places]);
  // WKK -- END DO NOT CHANGE

}
