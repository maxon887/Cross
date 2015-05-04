#ifndef FMOD_EXAMPLES_COMMON_H
#define FMOD_EXAMPLES_COMMON_H

#include "Game.h"
#include "fmod.h"

#ifndef Common_vsnprintf
    #define Common_vsnprintf vsnprintf
#endif

/* Cross platform functions (common) */
void Common_Fatal(const char *format, ...);
void Common_Draw(const char *format, ...);

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

/* Functions with platform specific implementation (common_platform) */
void Common_Close();
void Common_Format(char *buffer, int bufferSize, const char *formatString...);
const char *Common_MediaPath(const char *fileName);

#endif
