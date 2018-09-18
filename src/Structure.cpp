#include <windows.h>
#include <GL/GL.h>
#include "Structure.h"
#include "Log.h"

void Structure::Render()
{
	glPushMatrix();
		glTranslatef(Position.x(),Position.y(),Position.z());
		glRotatef(Rotation.x(),1,0,0);
		glRotatef(Rotation.y(),0,1,0);
		glRotatef(Rotation.z(),0,0,1);	
		model.Render();
	glPopMatrix();
}