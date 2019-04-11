#include "philosopher.h"
#include "table.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int
main ()
{
  int N;
  printf ("N ==> ");
  scanf ("%d", &N);
  int seed;
  printf ("seed ==> ");
  scanf ("%d", &seed);
  int time;
  printf ("time ==> ");
  scanf ("%d", &time);

  table_t* table = make_table (N);
  philosopher_t** phils = (philosopher_t**)malloc (N * sizeof (philosopher_t*));

  for (int p = 0; p < N; ++p) {
    sem_t* left = table->forks + p;
    sem_t* right = table->forks + (p + 1) % N;
    phils[p] = make_philosopher(p, left, right, table, seed);
  }

  pthread_t* threads = (pthread_t*)malloc (N * sizeof (pthread_t));

  for (int i = 0; i < N; ++i) {
    pthread_create(&threads[i], NULL, (void*)phil_run, phils[i]);
  }
  sleep (time);
  table->open = false;

  for (int i = 0; i < N; ++i) {
    pthread_join(threads[i], NULL);
  }

  bool valid = table->valid;
  if (valid) {
    puts("valid");
  } else {
    puts("invalid");
  }

  for (int i = 0; i < N; ++i) {
    free(phils[i]);
  }
  free_table(table);

  return valid ? EXIT_SUCCESS : EXIT_FAILURE;
}
