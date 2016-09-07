#ifndef MERC_DISPLAY_H
#define MERC_DISPLAY_H

#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/GL.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "RenderObject.h"
#include "Player.h"

class Display
{
private:
	RenderObject *Olist;
	int NumObjects;
	SDL_Surface *Screen;
	int Width;
	int Height;
public:
	Display();
	void AddObject(RenderAble *Obj);
	void Init();
	void PlaceCamera(Player *player);
	void Render();
	void DeleteObject(int id);
};

#endif