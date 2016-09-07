#ifndef MERC_VECTOR3_H
#define MERC_VECTOR3_H

// TODO: Implementar operador de asignacion
#include "Constants.h"

class Vector3
{
private:
	float mVector[3];
public:
	//CONSTRUCTORES
	//setea las coordenadas a 0.0f 0.0f 0.0f
	Vector3();
	
	//setea las coordenadas a X,Y,Z
	Vector3(float X,float Y,float Z);
	
	//constructor de copia
	Vector3(const Vector3 &v);

	//FUNCIONES
	//realiza el producto cruz(o vectorial)
	//entre el vector y v (Vector x v)
	Vector3 &Cross(const Vector3 &other);
	
	//realiza el producto punto(o escalar)
	//entre el vector y v
	float Dot(const Vector3 &other);
	
	//obtiene la norma(longitud) del vector
	float Length();

	//normaliza el vector
	void Normalize();

	//MUTADORES
	//obtiene la coordenada x
	float x(){return mVector[0];}
	
	//obtiene la coordenada y
	float y(){return mVector[1];}

	//obtiene la coordenada z
	float z(){return mVector[2];}

	//establece la coordenada x a X
	void x(float X){mVector[0] = X;}

	//establece la coordenada y a Y
	void y(float Y){mVector[1] = Y;}

	//establece la coordenada z a Z
	void z(float Z){mVector[2] = Z;}

	//solo se puede obtener el vector
	//usado principalmente para funciones como glVertex3fv
	float *GetVector(){ return(mVector);}

	//OPERADORES
	Vector3 &operator+(const Vector3 &v2);
	Vector3 &operator-(const Vector3 &v2);
	void operator*=(const float escalar);
	void operator+=(const Vector3 &v2);
	void operator-=(const Vector3 &v2);
	//friend Vector3 &operator=(const Vector3 v1,const Vector3 v2);
	bool operator==(const Vector3 &v2);
};
#endif
