#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"

void
queue_init (struct queue* q, int size)
{
  q->N = size;
  q->buf = malloc (sizeof (int) * size);
  q->begin = 0;
  q->end = 0;
  mutex_init(&q->mutex);
  sem_init(&q->slots, size);
  sem_init(&q->items, 0);
  // TODO: initialize synchronization primitives
}

void
queue_free (struct queue* q)
{
  free (q->buf);
  q->buf = NULL;
}

// TODO: For each of the following methods, add appropriate
// synchronization mechanisms

void
queue_add (struct queue* q, int val)
{
  sem_wait(&q->slots);
  mutex_wait(&q->mutex);
  q->buf[++q->end % q->N] = val;
  fprintf(stderr, "added %d\n", val);
  mutex_signal(&q->mutex);
  sem_signal(&q->items);
}

int
queue_remove (struct queue* q)
{
  int val;
  sem_wait(&q->items);
  mutex_wait(&q->mutex);
  val = q->buf[++q->begin % q->N];
  fprintf(stderr, "removed %d\n", val);
  mutex_signal(&q->mutex);
  sem_signal(&q->slots);
  return val;

}

int
queue_peek (struct queue* q)
{
  int val;
  sem_wait(&q->items);
  mutex_wait(&q->mutex);
  val = q->buf[(q->begin + 1) % q->N];
  mutex_signal(&q->mutex);
  sem_signal(&q->items);
  return val;
}
