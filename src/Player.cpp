#include <windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include "Player.h"
#include "Log.h"

void Player::Render()
{
	float sa;
	float angle;
	float axisx;
	float axisy;
	float axisz;

	/*sa=sqrtf(1-Q[0]*Q[0]);
	angle=2*acos(Q[0])*180/M_PI;
	axisx=Q[1]/sa;
	axisy=Q[2]/sa;
	axisz=Q[3]/sa;
*/

	//Log::Output("name %s %f %f %f\n",name,Position.x(),Position.y(),Position.z());
	glPushMatrix();
		glTranslatef(Position.x(),Position.y()+5,Position.z());
		//glRotatef(angle,axisx,axisy,axisz);
		glRotatef(90,1,0,0);
		GLUquadricObj *pObj = gluNewQuadric();	
		glColor3ub(0,255,0);
		gluQuadricDrawStyle(pObj, GLU_LINE);
		gluCylinder(pObj, 4, 4, 10, 15, 15);		
		gluDeleteQuadric(pObj);	
		glColor3ub(255,255,255);
	glPopMatrix();

}


void Player::LoadModel(char *modelname)
{
	model.Load(modelname);
}