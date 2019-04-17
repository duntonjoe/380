#ifndef TABLE_H_
#define TABLE_H_

#include <semaphore.h>
#include <stdbool.h>

struct philosopher;
typedef struct philosopher philosopher_t;

typedef struct table
{
  // WKK -- BEGIN DO NOT CHANGE

  // number of places at table
  int places;
  // indicator if table is still open
  bool open;
  // array of forks
  sem_t* forks;
  // indicator if problem occurred
  bool valid;
  // array for correctness
  int* access;

  // WKK -- END DO NOT CHANGE

  // TODO -- add whatever else you need

} table_t;

table_t*
make_table (int places);

void
free_table (table_t* t);

void
table_checkin (table_t* t, int id, philosopher_t* p);

void
table_checkout (table_t*, int id, philosopher_t* p);

#endif
