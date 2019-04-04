#ifndef MUTEX_H_
#define MUTEX_H_

struct mutex
{
  char lock;
};

void
mutex_init (volatile struct mutex* m);

void
mutex_wait (volatile struct mutex* m);

void
mutex_signal (volatile struct mutex* m);

#endif
