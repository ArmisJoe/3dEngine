
#pragma once
#include "Globals.h"

#include "glmath.h"
#include "Color.h"

#include "glew-2.1.0\include\GL\glew.h"

using namespace std;


enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();
	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire, visible;

protected:
	PrimitiveTypes type;

	uint num_vertices = 0;
	//vector<vec3> vertices;
	//vector<uint> indices;
};

// ============================================
class bCube : public Primitive
{
public :
	bCube();
	bCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
	void Start();
public:
	vec3 size;
private:
	uint vertices_id = 2, indices_id = 3;
	/*GLuint elements[36] = {
		2, 0 , 1,
		3, 2, 1,
		3, 1, 5,
		7, 3, 5,
		2, 3, 7,
		6, 4, 7,
		6, 4, 0,
		2, 6, 0,
		7, 5, 4,
		6, 7, 4,
		1, 0, 4,
		5, 1, 4
	};*/

	GLuint elements[36] = {
		2, 0 , 1,
		3, 2, 1,
		3, 1, 5,
		7, 3, 5,
		2, 3, 7,
		6, 2, 7,
		6, 4, 0,
		2, 6, 0,
		7, 5, 4,
		6, 7, 4,
		1, 0, 4,
		5, 1, 4
	};
};

// ============================================
class bSphere : public Primitive
{
public:
	bSphere();
	bSphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class bCylinder : public Primitive
{
public:
	bCylinder();
	bCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class bLine : public Primitive
{
public:
	bLine();
	bLine(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class bPlane : public Primitive
{
public:
	bPlane();
	bPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};