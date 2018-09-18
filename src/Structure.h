#ifndef MERC_STRUCTURE_H
#define MERC_STRUCTURE_H

#include "Vector3.h"
#include "RenderAble.h"
#include "Model.h"

class Structure:public RenderAble
{
public:
	Vector3 Position;
	Vector3 Rotation;
	Model model;
	Structure *next;
	Structure *prev;

	void Render();
};

#endif