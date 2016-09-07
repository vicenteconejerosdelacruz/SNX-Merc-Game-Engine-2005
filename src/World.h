#ifndef MERC_WORLD_H
#define MERC_WORLD_H

struct MapHeader{
	char TextureFile[64];
	char TextureDetailFile[64];
	int SkyDomeHeight;
	int SkyDomeRatio;
	char SkyDomeTextureFile[64];
	int sizeX;
	int sizeY;
};

class World
{
protected:
	unsigned char * HeightMap;
	int sizeX;
	int sizeY;
	char name[64];
public:
	virtual void Init(char *WorldName){};
	char *GetName(){return name;}
};

#endif