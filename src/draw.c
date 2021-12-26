#include "draw.h"

static SDL_Texture* buf;

void createRendTex()
{
	buf = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIN_X, WIN_Y);
}

void prepareMiniMap(SDL_Texture** tex)
{
	*tex = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 200);
	SDL_SetTextureBlendMode(*tex, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(app.renderer, *tex);
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(app.renderer, NULL);
	//SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);
}

void setRendToWin()
{
	SDL_SetRenderTarget(app.renderer, NULL);
}

SDL_Texture* loadTexture(char* filename)
{
	SDL_Texture* texture;

	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(app.renderer, filename);

	return texture;
}

void destroyTexture(SDL_Texture* texture)
{
	SDL_DestroyTexture(texture);
}

void prepareScene()
{
	SDL_SetRenderTarget(app.renderer, buf);
}

void presentMiniMap(SDL_Texture* tex)
{
	SDL_RenderPresent(app.renderer);
}

void presentScene(postProcess_t pp, SDL_Rect ppSrc)
{
	SDL_RenderPresent(app.renderer);

	SDL_Rect dest = { 30, 420, 300, 250 };
	if (pp == FACE_CAM)
		SDL_RenderCopy(app.renderer, buf, &ppSrc, &dest);

	SDL_SetRenderTarget(app.renderer, NULL);
	SDL_RenderCopy(app.renderer, buf, NULL, NULL);
}

void renderCopy(SDL_Texture* tex, SDL_Rect rect)
{
	SDL_RenderCopy(app.renderer, tex, NULL, &rect);
}

void drawLine(IntVector v1, IntVector v2)
{
	SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(app.renderer, v1.x - app.camera.x, v1.y - app.camera.y, v2.x - app.camera.x, v2.y - app.camera.y);
}

void drawLineColored(IntVector v1, IntVector v2, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetRenderDrawColor(app.renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(app.renderer, v1.x - app.camera.x, v1.y - app.camera.y, v2.x - app.camera.x, v2.y - app.camera.y);
}

void blit(SDL_Texture* texture, int x, int y, double rotation, float scale, SDL_RendererFlip flip)
{
	SDL_Rect dest = { x - app.camera.x , y - app.camera.y};
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	dest.w = (int) (dest.w * scale);
	dest.h = (int) (dest.h * scale);

	// this is incredibly hacky but the only thing that rotates is Batty so hardcode her center
	SDL_Point p = { 267 * scale, 400 * scale};

	SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, rotation, rotation ? &p : NULL, flip);
}

void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y)
{
	SDL_Rect dest = { x, y, src->w, src->h };

	SDL_RenderCopy(app.renderer, texture, src, &dest);
}

void drawBg(SDL_Texture* bg, SDL_Texture* bg2, SDL_Texture* bg3, int pos)
{
	int w = -1, h = -1;
	SDL_QueryTexture(bg, NULL, NULL, &w, &h);
	double scale = (double)WIN_Y / (double)h;
	int scaledW = w * scale;
	for (int x = 0; x < WIN_X; x += scaledW)
	{
		blit(bg, x + app.camera.x, 0 + app.camera.y, 0, scale, SDL_FLIP_NONE);
	}

	int offset = 0;

	if (bg3)
	{
		SDL_QueryTexture(bg3, NULL, NULL, &w, &h);
		scaledW = w * scale;
		offset = SDL_GetTicks() % (scaledW * 32);
		offset /= 32;
		//offset = (scaledW - offset);

		for (int x = -offset; x < WIN_X; x += scaledW)
			blit(bg3, x + app.camera.x, app.camera.y + (WIN_Y - h * scale), 0, scale, SDL_FLIP_NONE);
	}


	SDL_QueryTexture(bg2, NULL, NULL, &w, &h);

	scaledW = w * scale;

	offset = abs(pos) % scaledW;
	
	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "offset %d", offset);
	for (int x = -offset; x < WIN_X; x += scaledW)
	{
		blit(bg2, x + app.camera.x, app.camera.y + (WIN_Y - h * scale) + 200, 0, scale, SDL_FLIP_NONE);
	}
}