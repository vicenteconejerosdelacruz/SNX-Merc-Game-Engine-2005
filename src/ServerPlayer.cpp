#include <windows.h>
#include <GL/GL.h>
#include "ServerPlayer.h"
#include "Log.h"

void ServerPlayer::Render()
{
	glPushMatrix();
	glTranslatef(Position.x(),Position.y(),Position.z());
	model.Render();
	glPopMatrix();
	//Player::Render();
	//Log::Output("Rendering ClientPlayer %s\n",name);
}
