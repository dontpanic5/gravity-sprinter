#include "draw.h"

SDL_Texture* loadTexture(char* filename)
{
	SDL_Texture* texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(app.renderer, filename);

	return texture;
}

void prepareScene(void)
{
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
	SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
	SDL_RenderPresent(app.renderer);
}

void drawLine(IntVector v1, IntVector v2)
{
	SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(app.renderer, v1.x - app.camera.x, v1.y - app.camera.y, v2.x - app.camera.x, v2.y - app.camera.y);
}

void blit(SDL_Texture* texture, int x, int y, double rotation, float scale)
{
	SDL_Rect dest = { x - app.camera.x , y - app.camera.y};
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	dest.w = (int) (dest.w * scale);
	dest.h = (int) (dest.h * scale);

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, rotation, NULL, flip);
}

void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y)
{
	SDL_Rect dest = { x, y, src->w, src->h };

	SDL_RenderCopy(app.renderer, texture, src, &dest);
}