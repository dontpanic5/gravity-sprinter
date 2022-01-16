#include "draw.h"

static const SDL_Rect DEST = { 30, 420, 300, 250 };
static DoubleVector ppScale;
static SDL_Rect ppSrc;
static postProcess_t pp;
static SDL_Texture* ppTex;

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

void presentMiniMap(SDL_Texture* tex)
{
	SDL_RenderPresent(app.renderer);
}

void prepareScene(postProcess_t newPp, SDL_Rect newPpSrc)
{
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
	SDL_RenderClear(app.renderer);

	ppTex = SDL_CreateTexture(app.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 250);
	SDL_SetTextureBlendMode(ppTex, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(app.renderer, ppTex);
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(app.renderer, NULL);
	SDL_SetRenderTarget(app.renderer, NULL);

	pp = newPp;
	ppSrc = newPpSrc;
	ppScale.x = (double)DEST.w / (double)ppSrc.w;
	ppScale.y = (double)DEST.h / (double)ppSrc.h;
}

void presentScene(postProcess_t pp, SDL_Rect ppSrc)
{
	blit(ppTex, 30 + app.camera.x, 420 + app.camera.y, 0, 1, SDL_FLIP_NONE);
	destroyTexture(ppTex);
	SDL_RenderPresent(app.renderer);
}

void drawLine(IntVector v1, IntVector v2)
{
	SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(app.renderer, v1.x - app.camera.x, v1.y - app.camera.y, v2.x - app.camera.x, v2.y - app.camera.y);

	if (pp == FACE_CAM)
	{
		v1.x -= app.camera.x;
		v1.y -= app.camera.y;
		v2.x -= app.camera.x;
		v2.y -= app.camera.y;
		IntVector v1Mod = v1;
		IntVector v2Mod = v2;
		if (SDL_IntersectRectAndLine(&ppSrc, &v1Mod.x, &v1Mod.y, &v2Mod.x, &v2Mod.y))
		{
			v1.x -= ppSrc.x;
			v1.y -= ppSrc.y;
			v2.x -= ppSrc.x;
			v2.y -= ppSrc.y;

			SDL_SetRenderTarget(app.renderer, ppTex);

			SDL_RenderDrawLine(app.renderer, v1.x * ppScale.x, v1.y * ppScale.y, v2.x * ppScale.x, v2.y * ppScale.y);
			SDL_RenderDrawLine(app.renderer, v1.x * ppScale.x + 1, v1.y * ppScale.y + 1, v2.x * ppScale.x + 1, v2.y * ppScale.y + 1);
			SDL_RenderDrawLine(app.renderer, v1.x * ppScale.x + 2, v1.y * ppScale.y + 2, v2.x * ppScale.x + 2, v2.y * ppScale.y + 2);
			SDL_RenderDrawLine(app.renderer, v1.x * ppScale.x + 3, v1.y * ppScale.y + 3, v2.x * ppScale.x + 3, v2.y * ppScale.y + 3);
			SDL_RenderDrawLine(app.renderer, v1.x * ppScale.x + 4, v1.y * ppScale.y + 4, v2.x * ppScale.x + 4, v2.y * ppScale.y + 4);
			SDL_RenderDrawLine(app.renderer, v1.x * ppScale.x + 5, v1.y * ppScale.y + 5, v2.x * ppScale.x + 5, v2.y * ppScale.y + 5);

			SDL_SetRenderTarget(app.renderer, NULL);
		}
	}
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

	if (pp == FACE_CAM)
	{
		SDL_SetRenderTarget(app.renderer, ppTex);

		dest.x -= ppSrc.x;
		dest.y -= ppSrc.y;
		dest.x *= ppScale.x;
		dest.y *= ppScale.y;
		dest.w *= ppScale.x;
		dest.h *= ppScale.y;
		p.x *= ppScale.x;
		p.y *= ppScale.y;
		SDL_RenderCopyEx(app.renderer, texture, NULL, &dest, rotation, rotation ? &p : NULL, flip);

		SDL_SetRenderTarget(app.renderer, NULL);
	}
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