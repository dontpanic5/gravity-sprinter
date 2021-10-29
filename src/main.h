#if !defined(MAIN_H_INCLUDED)
#define MAIN_H_INCLUDED

#include <string.h>
#include <math.h>
#include <Windows.h>

#include "structs.h"
#include "init.h"
#include "draw.h"
#include "stage.h"

#define WIN_X 1280
#define WIN_Y 720

#define GAME_LOOP_DELAY 16

App app;

extern void cleanup(void);
extern void initSDL(void);
extern SDL_Texture* loadTexture(char* filename);
extern void prepareScene(void);
extern void presentScene(void);
extern void doInput(void);
extern void blit(SDL_Texture* texture, int x, int y, double rotation, float scale);
extern void initStage(void);

#endif