#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "mutex.h"

struct semaphore
{
  struct mutex mutex;
  struct mutex delay;
  int count;
};

void
sem_init (struct semaphore*, int avail);

void
sem_wait (struct semaphore*);

void
sem_signal (struct semaphore*);

#endif
