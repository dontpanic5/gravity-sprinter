#if !defined(STRUCTS_H_INCLUDED)
#define STRUCTS_H_INCLUDED

#ifdef SDL_DIR
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#define MAX_HITBOX_LINES 6

typedef struct {
	int x;
	int y;
} IntVector;

typedef struct {
	double x;
	double y;
} DoubleVector;

enum POST_PROCESS {
	FACE_CAM,
	NONE
};
typedef enum POST_PROCESS postProcess_t;

enum GRAVITY {
	G_EARTH,
	G_MOON,
};
typedef enum GRAVITY Gravity;
#define LAST_G 1

typedef struct {
	void (*logic) (void);
	void (*draw) (postProcess_t* pp, SDL_Rect* ppSrc);
} Delegate;

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	Delegate delegate;
	IntVector camera;
	int up;
	int right;
	int left;
	int space;
	int t;
	int g;

	int tickCount;

	int lastHouse;

	Gravity gravity;
} App;

typedef struct {
	IntVector pos;
	DoubleVector velocity;
	double rotation;

	IntVector hitbox[MAX_HITBOX_LINES];

	SDL_Texture* texture;

	int energy;
} Entity;

#endif