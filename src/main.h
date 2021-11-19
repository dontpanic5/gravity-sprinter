#if !defined(MAIN_H_INCLUDED)
#define MAIN_H_INCLUDED

#include <string.h>
#include <math.h>
#include <Windows.h>

#include "structs.h"
#include "init.h"
#include "draw.h"
#include "stage.h"
#include "text.h"
#include "sound.h"

#define WIN_X 1280
#define WIN_Y 720

#define GAME_LOOP_DELAY 16

#define CAM_BUF_BOTTOM 450
#define CAM_BUF_TOP 270
#define CAM_BUF_HORZ 630


// DEBUG OPTIONS

//#define MAX_ENERGY
//#define SPAWN_HOUSE 7
//#define NO_MUSIC
#define DRAW_HB


App app;

extern void cleanup(void);
extern void initSDL(void);
extern SDL_Texture* loadTexture(char* filename);
extern void prepareScene(void);
extern void presentScene(void);
extern void doInput(void);
extern void blit(SDL_Texture* texture, int x, int y, double rotation, float scale, SDL_RendererFlip flip);
extern void initStage(void);
extern void initTitle();
extern void initFonts(void);
extern void initSounds();
extern void loadMusic(char* filename);
extern void playMusic();

#endif