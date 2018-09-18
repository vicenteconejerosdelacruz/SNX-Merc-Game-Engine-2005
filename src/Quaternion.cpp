#include <math.h>
#include "Quaternion.h"
#include "MercError.h"

Quaternion::Quaternion()
{
	for(int i=0;i<4;i++)
		mQuaternion[i]=0.0f;
}

Quaternion::Quaternion(float W,float X,float Y,float Z)
{
	mQuaternion[0]=W;
	mQuaternion[1]=X;
	mQuaternion[2]=Y;
	mQuaternion[3]=Z;
}

Quaternion::Quaternion(const Quaternion &q)
{
	mQuaternion[0]=q.mQuaternion[0];
	mQuaternion[1]=q.mQuaternion[1];
	mQuaternion[2]=q.mQuaternion[2];
	mQuaternion[3]=q.mQuaternion[3];
}

void Quaternion::Multiply(const Quaternion &q)
{
	const float rw = qw*q.qw - qx*q.qx - qy*q.qy - qz*q.qz;
	const float rx = qw*q.qx + qx*q.qw + qy*q.qz - qz*q.qy;
	const float ry = qw*q.qy - qx*q.qz + qy*q.qw + qz*q.qx;
	const float rz = qw*q.qz + qx*q.qy - qy*q.qx + qz*q.qw;
	qw = rw;
	qx = rx;
	qy = ry;
	qz = rz;
}

Quaternion Quaternion::Dot(const Quaternion &q)
{
	return Quaternion(qw*q.qw + qx*q.qx + qy*q.qy + qz*q.qz, 0, 0, 0);
}

float Quaternion::Length()
{
	float Sum=0.0f;

	for(int i=0;i<4;i++)
		Sum+=mQuaternion[i]*mQuaternion[i];

	return sqrtf(Sum);
}

float Quaternion::Norm()
{
	float Sum=0.0f;

	for(int i=0;i<4;i++)
		Sum+=mQuaternion[i]*mQuaternion[i];

	return Sum;
}

void Quaternion::Normalize()
{
	const float length = this->Length();

	if (length == 0)
	{
		qw = 1;
		qx = 0;
		qy = 0;
		qz = 0;
	}
	else
	{
		float inv = 1.0f / length;
		qx = qx * inv;
		qy = qy * inv;
		qz = qz * inv;
		qw = qw * inv;
	}
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion(qw, -qx, -qy, -qz);
}

Quaternion Quaternion::Inverse()
{
	const float n = Norm();
	if(n==0)
		throw_error("norma = 0","norma = 0",1);

	return Quaternion(qw/n, -qx/n, -qy/n, -qz/n); 
}

Quaternion &Quaternion::operator+(const Quaternion &q2)
{	
	Quaternion *suma = new Quaternion();

	for(int i=0;i < 4;i++)
		suma->mQuaternion[i] = mQuaternion[i]+q2.mQuaternion[i];
	return(*suma);
}

Quaternion &Quaternion::operator-(const Quaternion &q2)
{	
	Quaternion *suma = new Quaternion();

	for(int i=0;i < 4;i++)
		suma->mQuaternion[i] = mQuaternion[i]-q2.mQuaternion[i];
	return(*suma);
}

void Quaternion::operator *=(const float escalar){

	for(int i=0;i<4;i++)
		mQuaternion[i]*=escalar;
}

void Quaternion::operator+=(const Quaternion &q2)
{
	for(int i=0;i<4;i++)
		mQuaternion[i]+=q2.mQuaternion[i];
}

void Quaternion::operator-=(const Quaternion &q2)
{
	for(int i=0;i<4;i++)
		mQuaternion[i]-=q2.mQuaternion[i];
}

bool Quaternion::operator==(const Quaternion &q2)
{
	bool result = true;

	for(int i=0;i < 4;i++){
		if(fabs(mQuaternion[i] - q2.mQuaternion[i]) > EPSILON){
			result = false;
		}
	}
	
	return(result);
}