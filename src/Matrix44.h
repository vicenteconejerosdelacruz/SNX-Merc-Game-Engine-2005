#ifndef MERC_MATRIX44_H
#define MERC_MATRIX44_H

//TODO: Declarar e implementar operadores
#include "Vector3.h"

class Matrix44
{
	float mMatrix[4][4];
public:
	//constructor por defecto y de copia
	Matrix44();
	Matrix44(const Matrix44 &other);
	
	//FUNCIONES
	//Resetea a la matriz identidad
	void LoadIdentity();

	//Construye una matriz de transformacion de solo rotacion en torno a x,y,z
	void BuildRotation(float rotx,float roty,float rotz);

	//Construye una matriz de la inversa de la transformacion 
	//de rotacion en torno a x,y,z
	void BuildIRotation(float rotx,float roty,float rotz);

	//Construye una matriz de transformacion de solo traslacion
	void BuildTranslation(float rotx,float roty,float rotz);

	//Construye una matriz de transformacion de traslacion x,y,z 
	//y rotacion rotx,roty,rotz
	void BuildTransformation(float rotx,float roty,float rotz,
								float x,float y,float z);

	//Retorna la transformada del vector v
	Vector3 &Transform(Vector3 &v);

	//Retorna la transformada inversa del vector v
	Vector3 &InverseTransform(Vector3 &v);

	//Retorna el vector transformado usando la 
	//transformada de solo rotacion de la matriz
	Vector3 &Rotate(Vector3 &v);

	//Retorna el vector transformado usando la 
	//transformada inversa de rotacion de la matriz
	Vector3 &InverseRotate(Vector3 &v);

	//Producto de matrices, tambien llamada concatenacion de matrices
	Matrix44 &Concat(Matrix44 &m);
};

#endif
