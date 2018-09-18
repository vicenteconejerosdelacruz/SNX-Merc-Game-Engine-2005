#ifndef MERC_SKYDOME_H
#define MERC_SKYDOME_H

#include<math.h>
#include"RenderAble.h"
#include"Vector3.h"

typedef struct VertexNode
{
	int Down;
	int Right;
}VertexNode;

typedef struct Vertex
{
	Vector3 Position;
	float u,v;
}Vertex;

class SkyDome:public RenderAble
{
private:
	int NumArcs;
	int NumRings;// Paralelas

	Vertex *Vertices;
	VertexNode *VertexPtr;

	Vector3 Position;

	int NumVertexes;
	int NumVertexesPointer;

	unsigned int Texture;
public:
	void Init(char *texturename,int Radius,int Rings,int Arcs);//Paralelas,Meridianos
	void Render();
	void Place(Vector3 &Position);
};

#endif 
