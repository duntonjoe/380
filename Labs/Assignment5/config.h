#ifndef CONFIG_H_
#define CONFIG_H_

#ifndef SLEEP_FACTOR
#define SLEEP_FACTOR 500000
#endif

#ifndef EAT_FACTOR
#define EAT_FACTOR 500000
#endif

#ifndef ARRIVE_FACTOR
#define ARRIVE_FACTOR 500000
#endif

#if defined(NDEBUG)
#define DEBUG_PRINT(...) do { } while (false)
#else
#define DEBUG_PRINT(...) fprintf (stdout, __VA_ARGS__)
#endif

#endif
