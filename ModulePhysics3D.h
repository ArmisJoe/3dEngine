#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include "src\MathGeoLib.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

#include <list>

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class DebugDrawer;
struct PhysBody3D;
struct PhysVehicle3D;
struct VehicleInfo;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void Draw();

	Sphere* AddSphere(const float &x, const float &y, const float &z, float radius, bool visible = true);
	AABB* AddAABB(const float3 center, const float3 size, bool visible = true);
	OBB* AddOBB();
	Line* AddLine();
	LineSegment AddLineSegment();
	Capsule* AddCapsule();
	Frustum* AddFrustum();
	Plane* AddPlane();
	Polyhedron* AddPolyhedron();
	Triangle* AddTriangle();
	Ray* AddRay();

	int CheckIntersec(Sphere* sp);

	void SetToWire(bool flag);

	void CheckAllIntersec();

	/*PhysBody3D* AddBody(const Sphere& sphere, float mass = 1.0f);
	PhysBody3D* AddBody(const Cube& cube, float mass = 1.0f);
	PhysBody3D* AddBody(const Cylinder& cylinder, float mass = 1.0f);
	PhysVehicle3D* AddVehicle(const VehicleInfo& info);

	void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, bool disable_collision = false);
*/

	std::list<Sphere*> spheres;
	std::list<AABB*> aabbs;
	std::list<OBB*> obbs;
	std::list<Line*> lines;
	std::list<LineSegment*> line_segments;
	std::list<Capsule*> capsules;
	std::list<Frustum*> frustums;
	std::list<Plane*> planes;
	std::list<Polyhedron*> polyhedrons;
	std::list<Triangle*> triangles;

	std::list<Ray*> rays;

	std::list<Primitive*> g_primitives;

private:

	bool debug;

	/*
	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld*			world;
	btDefaultVehicleRaycaster*			vehicle_raycaster;

	list<btCollisionShape*> shapes;
	list<PhysBody3D*> bodies;
	list<btDefaultMotionState*> motions;
	list<btTypedConstraint*> constraints;
	list<PhysVehicle3D*> vehicles;
	*/
	DebugDrawer*						debug_draw;
};


class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	bLine line;
	Primitive point;
};