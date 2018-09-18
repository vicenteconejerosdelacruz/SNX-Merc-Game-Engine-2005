#ifndef MERC_RENDEROBJECT_H
#define MERC_RENDEROBJECT_H

#include "RenderAble.h"

class RenderObject
{
public:
	RenderAble *Object;
	RenderObject *next;
	RenderObject *prev;
	bool State;
};

#endif
