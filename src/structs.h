#if !defined(STRUCTS_H_INCLUDED)
#define STRUCTS_H_INCLUDED

#include "SDL2/SDL.h"

#define MAX_HITBOX_LINES 6

typedef struct {
	int x;
	int y;
} IntVector;

typedef struct {
	double x;
	double y;
} DoubleVector;

typedef struct {
	void (*logic) (void);
	void (*draw) (void);
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

	int tickCount;

	int lastHouse;
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