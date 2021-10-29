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
	SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
	SDL_RenderClear(app.renderer);
}

void presentScene(void)
{
	SDL_RenderPresent(app.renderer);
}

void blit(SDL_Texture* texture, int x, int y, double rotation, float scale)
{
	SDL_Rect dest = { x, y };
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