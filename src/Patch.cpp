#include "Patch.h"
#include "Log.h"
#include "Vector3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Patch::Patch()
{
	HeightPtr=NULL;
	worldX=0;
	worldY=0;
	memset(Normal,0,sizeof(Normal));
}

Patch::~Patch()
{

}

void Patch::Init(unsigned char *HMapPointer,int MapworldX,int MapworldY,int SizeX,int SizeY)
{
	HeightPtr=HMapPointer;
	worldX=MapworldX;
	worldY=MapworldY;
	MapSizeX=SizeX;
	MapSizeY=SizeY;

	Vector3 Vector1;
	Vector3 Vector2;
	Vector3 Cross;
	
	Vector1.x(PATCH_SIZE);
	Vector1.y((float)HeightPtr[PATCH_SIZE]-(float)HeightPtr[0]);
	Vector1.z(0);

	Vector2.x(0);
	Vector2.y((float)HeightPtr[1024*PATCH_SIZE]-(float)HeightPtr[0]);
	Vector2.z(PATCH_SIZE);

	Cross=Vector1.Cross(Vector2);

	Normal[0]=Cross.x();
	Normal[1]=Cross.y();
	Normal[2]=Cross.z();
}

int Patch::GetVertexes(float *Vertexes,float *TexCoordPointer,float *NormalPointer)
{
	Vertexes[0]=(float)(worldX);
	Vertexes[1]=(float)(HeightPtr[0]);
	Vertexes[2]=(float)(worldY);

	Vertexes[3]=(float)(worldX+PATCH_SIZE);
	Vertexes[4]=(float)(HeightPtr[PATCH_SIZE]);
	Vertexes[5]=(float)(worldY);

	Vertexes[6]=(float)(worldX+PATCH_SIZE);
	Vertexes[7]=(float)(HeightPtr[MapSizeX*PATCH_SIZE+PATCH_SIZE]);
	Vertexes[8]=(float)(worldY+PATCH_SIZE);

	Vertexes[9]=(float)(worldX);
	Vertexes[10]=(float)(HeightPtr[MapSizeX*PATCH_SIZE]);
	Vertexes[11]=(float)(worldY+PATCH_SIZE);

	TexCoordPointer[0]=(float)worldX/MapSizeX;
	TexCoordPointer[1]=(float)worldY/MapSizeY;
	TexCoordPointer[2]=(float)(worldX+PATCH_SIZE)/MapSizeX;
	TexCoordPointer[3]=(float)worldY/MapSizeY;
	TexCoordPointer[4]=(float)(worldX+PATCH_SIZE)/MapSizeX;
	TexCoordPointer[5]=(float)(worldY+PATCH_SIZE)/MapSizeY;
	TexCoordPointer[6]=(float)worldX/MapSizeX;
	TexCoordPointer[7]=(float)(worldY+PATCH_SIZE)/MapSizeY;

	for(int i=0;i<3;i++)
		NormalPointer[i]=Normal[i];

	return 12;
}
