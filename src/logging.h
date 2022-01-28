#ifndef LOGGING_H_INIT

#ifdef SDL_DIR
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

void batLog(const char* format, ...);

#endif // !LOGGING_H_INIT
