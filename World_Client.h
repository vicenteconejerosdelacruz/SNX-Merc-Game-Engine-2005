#ifndef MERC_WORLDCLIENT_H
#define MERC_WORLDCLIENT_H

#include "World.h"
#include "RenderAble.h"
#include "Patch.h"
#include "TriNode.h"

#define POOL_SIZE				9000
#define PATCH_SIZE				16

class World_Client:public World,public RenderAble
{
private:
	Patch *Patches;
	TriNode TNodePool[POOL_SIZE];
	int NumTextures;
	unsigned int *Texture;
	int NumPatches;
public:
	World_Client();
	void Init(char *WorldName);
	int GetNumTextures();
	unsigned int GetTexture(int index);
	void Render();
	void Reset();
	void Tesselate();
	void PlaceCamera(float *Position,float *Rotation);
};

#endif