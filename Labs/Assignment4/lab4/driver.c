#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"
#include "config.h"

// atomic integers used for thread termination
_Atomic int read_left;
_Atomic int write_left;

// argument struct for threads
struct thread_args
{
  struct queue* q;
  int id;
};

// "reader" thread function
void
reader (void* args)
{
  struct thread_args* a = (struct thread_args*)args;
  struct queue* q = a->q;
  free (args);
  while (1)
  {
    if (--read_left < 0)
    {
      break;
    }
    queue_remove (q);
    PAUSE ();
  }
}

// "writer" thread function
void
writer (void* args)
{
  struct thread_args* a = (struct thread_args*)args;
  struct queue* q = a->q;
  int id = a->id;
  free (args);
  while (1)
  {
    if (--write_left < 0)
    {
      break;
    }
    queue_add (q, id);
    PAUSE ();
  }
}

// helper function to make a thread with a ptr to the queue, an id, and the function
pthread_t
make_thread (struct queue* q, int id, void* fn)
{
  struct thread_args* args = malloc (sizeof (struct thread_args));
  args->id = id;
  args->q = q;
  pthread_t tid;
  pthread_create (&tid, NULL, fn, args);
  return tid;
}

int
main ()
{
  // number of threads which read data from the queue
  int readers;
  printf ("Readers ==> ");
  assert (scanf ("%d", &readers) == 1);

  // number of threads which write data to the queue
  int writers;
  printf ("Writers ==> ");
  assert (scanf ("%d", &writers) == 1);

  // buffer size of the queue
  int size;
  printf ("Buffer Size ==> ");
  assert (scanf ("%d", &size) == 1);

  // number of total elements to insert/remove to/from the queue
  int count;
  printf ("Elements ==> ");
  assert (scanf ("%d", &count) == 1);

  read_left = count;
  write_left = count;

  int total = readers + writers;

  // intialize the queue
  struct queue q;
  queue_init (&q, size);

  // thread array
  pthread_t* tids = malloc (sizeof (pthread_t) * total);

  // pointer to current "write" location in the tid array
  pthread_t* tid = tids;

  // launch the writer threads
  for (int i = 0; i < writers; ++i)
  {
    *tid++ = make_thread (&q, i, writer);
  }

  // launch the reader threads
  for (int i = 0; i < readers; ++i)
  {
    *tid++ = make_thread (&q, i, reader);
  }

  // wait for all of the threads to finish
  for (int i = 0; i < total; ++i)
  {
    pthread_join (tids[i], NULL);
  }
  return 0;
}
