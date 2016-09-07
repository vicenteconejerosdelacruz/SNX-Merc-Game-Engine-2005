#ifndef MERC_WORLDSERVER_H
#define MERC_WORLDSERVER_H

#include "World.h"
#include "PhysicHandler.h"

class World_Server:public World
{
public:
	PhysicHandler PhManager;
	World_Server *next;
	World_Server *prev;

	World_Server();
	void Init(char *WorldName);
};

#endif