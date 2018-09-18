#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/GLEXT.h>
#include <math.h>
#include "SkyDome.h"
#include "Constants.h"
#include "Cache.h"
#include "Log.h"

extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
extern PFNGLMULTITEXCOORD2FPROC		glMultiTexCoord2fARB;

void SkyDome::Init(char *texturename,int Radius,int Rings,int Arcs)
{
	float drho;
	float dtheta;

	NumArcs=Arcs;
	NumRings=Rings;

	Vertices=new Vertex[NumRings*NumArcs+1];
	VertexPtr=new VertexNode[NumRings*NumArcs+1];

	NumVertexes=NumRings*NumArcs+1;
	NumVertexesPointer=NumRings*NumArcs+1;

	Vertices[0].Position.x(0.0f);
	Vertices[0].Position.y((float)Radius);
	Vertices[0].Position.z(0.0f);
	Vertices[0].u=0.5f;
	Vertices[0].v=0.5f;

	drho=90.0f/NumRings;
	dtheta=360.0f/NumArcs;

	Cache::LoadFile(texturename,TEXTURE_FILE,&Texture);

	for(int i=1;i<NumRings*NumArcs+1;i++)
		{
		int rest=i%NumArcs;

		if(i<=(NumRings*NumArcs-NumArcs))
			if(rest==1)
				VertexPtr[i].Down=i+NumArcs*2-1;
			else
				VertexPtr[i].Down=i+NumArcs-1;
		else
			VertexPtr[i].Down=-1;

		if(rest==0)
			VertexPtr[i].Right=i-NumArcs+1;
		else
			VertexPtr[i].Right=i+1;
		}

	
	for(int i=1;i<NumRings+1;i++)
	{
		float rho=i*drho;
		for(int e=0;e<NumArcs;e++)
		{	
			int pos=(i-1)*NumArcs+e+1;
			float theta=e*dtheta;
			float x,y,z;
			float u,v;

			z=(float)Radius*(float)sin(rho*(PI/180))*(float)cos(theta*(PI/180));
			x=(float)Radius*(float)sin(rho*(PI/180))*(float)sin(theta*(PI/180));
			y=(float)Radius*(float)cos(rho*(PI/180));

			u=sqrtf(0.20f)*x/Radius+0.5;
			v=sqrtf(0.20f)*z/Radius+0.5;

			Vertices[pos].u=u;
			Vertices[pos].v=v;
			
			Vertices[pos].Position.x(x);
			Vertices[pos].Position.y(y);
			Vertices[pos].Position.z(z);
		}
	}


}

void SkyDome::Render()
{
	int Iters=(NumVertexes-1)/NumRings;

	glPushMatrix();
	glTranslatef(Position.x(),Position.y(),Position.z());

	glColor3ub(255,255,255);
		
	glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D,Texture);

	for(int i=0;i<Iters;i++)
		{
		int NextNode=0;

		glBegin(GL_TRIANGLE_STRIP);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,
			Vertices[NextNode].u,
			Vertices[NextNode].v);
		glVertex3f(Vertices[NextNode].Position.x(),
			Vertices[NextNode].Position.y(),
			Vertices[NextNode].Position.z());

		NextNode=i+1;
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,
			Vertices[NextNode].u,
			Vertices[NextNode].v);
		glVertex3f(Vertices[NextNode].Position.x(),
			Vertices[NextNode].Position.y(),
			Vertices[NextNode].Position.z());

		for(int e=0;e<NumRings;e++)
			{
			NextNode=VertexPtr[NextNode].Right;
			
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,
				Vertices[NextNode].u,
				Vertices[NextNode].v);
			glVertex3f(Vertices[NextNode].Position.x(),
				Vertices[NextNode].Position.y(),
				Vertices[NextNode].Position.z());

			NextNode=VertexPtr[NextNode].Down;

			if(NextNode==-1)
				break;

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,
				Vertices[NextNode].u,
				Vertices[NextNode].v);
			glVertex3f(Vertices[NextNode].Position.x(),
				Vertices[NextNode].Position.y(),
				Vertices[NextNode].Position.z());
			}
		glEnd();
		}

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void SkyDome::Place(Vector3 &Pos)
{
	Position.x(Pos.x());
	Position.y(Pos.y()-800);
	Position.z(Pos.z());
}