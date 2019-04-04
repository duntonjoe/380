#ifndef CONFIG_H_
#define CONFIG_H_

// Difficulty level -- the lower this is, the longer the pauses will be
#ifndef DIFFICULTY
#define DIFFICULTY 0
#endif

#if DIFFICULTY == 0
#define PAUSE() sleep (1)
#elif DIFFICULTY == 1
#define PAUSE() usleep (100000)
#elif DIFFICULTY == 2
#define PAUSE() usleep (10000)
#elif DIFFICULTY == 3
#define PAUSE() usleep (1000)
#elif DIFFICULTY == 4
#define PAUSE() do { } while (false);
#endif

#endif
