#ifndef MERC_QUATERNION_H
#define MERC_QUATERNION_H

// TODO: Implementar operador de asignacion
#include "Constants.h"

class Quaternion
{
private:
	union{
		struct {float qw,qx,qy,qz;};
		float mQuaternion[4];
	};
public:
	//CONSTRUCTORES
	//setea las coordenadas a 0.0f 0.0f 0.0f 0.0f
	Quaternion();
	
	//setea el quaternion a W,X,Y,Z
	Quaternion(float W,float X,float Y,float Z);
	
	//constructor de copia
	Quaternion(const Quaternion &q);

	void Multiply(const Quaternion &q);

	//realiza el producto punto(o escalar)
	//entre el quaternion y q
	Quaternion Dot(const Quaternion &q);
	
	//obtiene la norma(longitud) del vector
	float Length();
	
	float Norm();

	//normaliza el quaternion
	void Normalize();
	
	//MUTADORES
	//obtiene la coordenada w
	float w(){return mQuaternion[0];}

	//obtiene la coordenada x
	float x(){return mQuaternion[1];}
	
	//obtiene la coordenada y
	float y(){return mQuaternion[2];}

	//obtiene la coordenada z
	float z(){return mQuaternion[3];}

	//establece la coordenada x a X
	void w(float W){mQuaternion[0] = W;}

	//establece la coordenada x a X
	void x(float X){mQuaternion[1] = X;}

	//establece la coordenada y a Y
	void y(float Y){mQuaternion[2] = Y;}

	//establece la coordenada z a Z
	void z(float Z){mQuaternion[3] = Z;}

	//solo se puede obtener el vector
	//usado principalmente para funciones como glVertex3fv
	float *GetQuaternion(){ return(mQuaternion);}

	Quaternion Conjugate();
	Quaternion Inverse();

	//OPERADORES
	Quaternion &operator+(const Quaternion &q2);
	Quaternion &operator-(const Quaternion &q2);
	void operator*=(const float escalar);
	void operator+=(const Quaternion &q2);
	void operator-=(const Quaternion &q2);
	//friend Vector3 &operator=(const Vector3 v1,const Vector3 v2);
	bool operator==(const Quaternion &q2);
};
#endif
