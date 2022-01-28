#if !defined(TITLE_H_INIT)
#define TITLE_H_INIT

#include "draw.h"
#include "input.h"
#ifdef SDL_DIR
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include "stage.h"
#include "instructions.h"
#include "structs.h"

extern App app;

extern void doInput(void);
extern void drawText(int x, int y, int r, int g, int b, int align, char* format, ...);
extern void initStage(void);
extern void initInstructions(void);

#endif