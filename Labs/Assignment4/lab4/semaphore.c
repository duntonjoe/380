#include "semaphore.h"

void
sem_init (struct semaphore* s, int avail)
{
  mutex_init (&s->mutex);
  mutex_init (&s->delay);
  mutex_wait (&s->delay);
  s->count = avail;
}

void
sem_wait (struct semaphore* s)
{
  mutex_wait (&s->mutex);
  s->count -= 1;
  if(s->count < 0){
	mutex_signal(&s->mutex);
	mutex_wait(&s->delay);
  }
  mutex_signal(&s->mutex);
}

void
sem_signal (struct semaphore* s)
{
  // TODO
  mutex_wait(&s->mutex);
  s->count += 1;
  if(s->count <= 0){
	mutex_signal(&s->delay);
  }
  else{
	mutex_signal(&s->mutex);
  }
}
