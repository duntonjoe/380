#include <sched.h>
#include <unistd.h>

#include "mutex.h"

#define SET ((char)1)
#define CLEAR ((char)0)

#define LOCK(Mutex) __sync_lock_test_and_set (&(Mutex)->lock, SET)
#define UNLOCK(Mutex) __sync_lock_release (&(Mutex)->lock)
#define ACQUIRE(Mutex) (LOCK (Mutex) ? spin_lock (Mutex) : CLEAR)
#define RELEASE(Mutex) UNLOCK (Mutex)

// limit of 2^6 times before yield
#define SPIN_LIMIT ((1 << 6) - 1)

static void
spin_lock (volatile struct mutex* m)
{
  int spins = 0;
  while (m->lock != CLEAR || LOCK (m))
  {
    if (!(++spins & SPIN_LIMIT))
    {
      // relinquish thread
      sched_yield ();
    }
  }
}

void
mutex_init (volatile struct mutex* m)
{
  m->lock = CLEAR;
}

void
mutex_wait (volatile struct mutex* m)
{
  ACQUIRE (m);
}

void
mutex_signal (volatile struct mutex* m)
{
  RELEASE (m);
}
