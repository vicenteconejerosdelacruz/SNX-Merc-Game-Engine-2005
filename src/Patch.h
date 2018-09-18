#ifndef MERC_PATCH_H
#define MERC_PATCH_H

#define PATCH_SIZE 16

class Patch  
{
private:
	unsigned char *HeightPtr;
	int worldX;
	int worldY;
	float Normal[3];
	int MapSizeX;
	int MapSizeY;
public:
	int GetVertexes(float *Vertexes,float *TexCoordPointer,float *NormalPointer);
	Patch();
	virtual ~Patch();
	void Init(unsigned char *HMapPointer,int MapworldX,int MapworldY,int SizeX,int SizeY);

};
#endif