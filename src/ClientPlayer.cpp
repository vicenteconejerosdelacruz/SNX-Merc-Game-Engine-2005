#include <windows.h>
#include <GL/GL.h>
#include "ClientPlayer.h"
#include "Log.h"

void ClientPlayer::Render()
{
	glPushMatrix();
	glTranslatef(Position.x(),Position.y(),Position.z());
	model.Render();
	glPopMatrix();
	//Player::Render();
	//Log::Output("Rendering ClientPlayer %s\n",name);
}
