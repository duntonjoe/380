#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>

#include "mutex.h"
#include "semaphore.h"

struct queue
{
  int* buf;
  int N;
  int begin;
  int end;
  struct mutex mutex;
  struct semaphore slots;
  struct semaphore items;

  // TODO: add primitives here
  //
  // HINTS:
  // 1). consider safe access to the buffer
  // 2). when are we "safe" to add an element?
  // 3). when are we "safe" to remove an element?
  //
  // Instead of keeping track of "size" we can use counting
  // semaphores to tell us how many elements we have in our
  // buffer and how many free slots are in our buffer.
};

void
queue_init (struct queue*, int size);

void
queue_free (struct queue*);

void
queue_add (struct queue*, int val);

int
queue_remove (struct queue*);

int
queue_peek (struct queue*);

#endif
