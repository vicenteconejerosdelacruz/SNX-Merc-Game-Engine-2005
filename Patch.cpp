#include <stdio.h>
#include "Patch.h"

Patch::Patch()
{
	BaseLeft.leftChild=NULL;
	BaseLeft.leftNeighbor=NULL;
	BaseLeft.rightChild=NULL;
	BaseLeft.rightNeighbor=NULL;

	BaseRight.leftChild=NULL;
	BaseRight.leftNeighbor=NULL;
	BaseRight.rightChild=NULL;
	BaseRight.rightNeighbor=NULL;
}

void Patch::Init(int heightX,int heightY,int worldX,int worldY,int mapsizeX,int mapsizeY,unsigned char *map)
{
	BaseLeft.baseNeighbor=&BaseRight;
	BaseRight.baseNeighbor=&BaseLeft;
	MapSizeX=mapsizeX;
	MapSizeY=mapsizeY;

	HeightPtr=&map[heightY*MapSizeX+heightX];

	worldX=worldX;
	worldY=worldY;

	Visible=0;
	VarianceDirty=1;
}

void Patch::ComputeVariance()
{

}