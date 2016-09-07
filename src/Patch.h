#ifndef MERC_PATCH_H
#define MERC_PATCH_H

#include "TriNode.h"

#define VARIANCE_DEPTH				9

class Patch  
{
private:
	unsigned char *HeightPtr;
	int worldX;
	int worldY;
	float Normal[3];
	int MapSizeX;
	int MapSizeY;
	TriNode BaseLeft;
	TriNode BaseRight;
	unsigned char VarianceLeft[1<<(VARIANCE_DEPTH)];
	unsigned char VarianceRight[1<<(VARIANCE_DEPTH)];
	unsigned char Visible;
	unsigned char VarianceDirty;
public:
	Patch();
	~Patch(){};
	void Split(TriNode *tri);
	TriNode *GetBaseLeft();
	TriNode *GetBaseRight();
	void Init(int heightX,int heightY,int worldX,int worldY,int mapsizeX,int mapsizeY,unsigned char *map);
	void Render();
	void Tesselate();
	void RecursiveTesselate(TriNode *tri,int leftX,int leftY,int rightX,int rightY,int apexX,							int apexY,int node);
	void Reset();
	void ComputeVariance();
	void RecursiveComputeVariance();
	void SetVisibility(int eyeX,int eyeY,int leftX,int leftY,int rightX,int rightY);
	bool isVisible();
	bool isDirty();
};

#endif