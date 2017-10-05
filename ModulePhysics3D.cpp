#include "Globals.h"
#include "Application.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "PanelConsole.h"

#ifdef _DEBUG
	#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
	#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics3D::ModulePhysics3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;

	name = "Physics3D";

/*
	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
*/
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;
	/*
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;*/
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{

	LOG("Creating 3D Physics simulation");
	bool ret = true;

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	/*world->setDebugDrawer(debug_draw);
	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setGravity(GRAVITY);
	vehicle_raycaster = new btDefaultVehicleRaycaster(world);

	// Big plane as ground
	{
		btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}
	*/
/*
	bCube * TestCube = new bCube(2.f, 2.f, 2.f);
	g_primitives.push_back(TestCube);
*/

	AddAABB({ 0,0,0 }, { 2, 2, 2 });

	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	/*
	world->stepSimulation(dt, 15);

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for(int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if(numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if(pbodyA && pbodyB)
			{
				for (list<Module*>::iterator it = pbodyA->collision_listeners.begin(); it != pbodyA->collision_listeners.end(); ++it)
				{
					(*it)->OnCollision(pbodyA, pbodyB);
				}
				for (list<Module*>::iterator it = pbodyB->collision_listeners.begin(); it != pbodyB->collision_listeners.end(); ++it)
				{
					(*it)->OnCollision(pbodyB, pbodyA);
				}
			}
		}
	}*/

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{
	/*
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		world->debugDrawWorld();

		// Render vehicles
		for (list<PhysVehicle3D*>::iterator it = vehicles.begin(); it != vehicles.end(); it++)
		{
			(*it)->Render();
		}
	}*/
	/*
	float sx = 2 * 0.5f;
	float sy = 2 * 0.5f;
	float sz = 2 * 0.5f;

	static GLfloat vertices_g[] = {
		-sx, -sy, -sz ,	//A
		-sx, -sy, sz ,		//B
		-sx, sy, -sz ,		//C
		-sx, sy, sz ,		//D
		sx, -sy, -sz ,		//E
		sx, -sy, sz ,		//F
		sx, sy, -sz ,		//G
		sx, sy, sz			//H
	};

	uint vertices_id = 1;
	glGenBuffers(1, (GLuint*) &(vertices_id));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, vertices_g, GL_STATIC_DRAW);


	static GLuint elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
	};

	uint indices_id = 1;
	*/
	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{

	if (!spheres.empty()) {
		for (std::list<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	spheres.clear();

	if (!aabbs.empty()) {
		for (std::list<AABB*>::iterator it = aabbs.begin(); it != aabbs.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	aabbs.clear();

	if (!obbs.empty()) {
		for (std::list<OBB*>::iterator it = obbs.begin(); it != obbs.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	obbs.clear();

	if (!lines.empty()) {
		for (std::list<Line*>::iterator it = lines.begin(); it != lines.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	lines.clear();

	if (!line_segments.empty()) {
		for (std::list<LineSegment*>::iterator it = line_segments.begin(); it != line_segments.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	line_segments.clear();

	if (!capsules.empty()) {
		for (std::list<Capsule*>::iterator it = capsules.begin(); it != capsules.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	capsules.clear();

	if (!frustums.empty()) {
		for (std::list<Frustum*>::iterator it = frustums.begin(); it != frustums.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	frustums.clear();

	if (!planes.empty()) {
		for (std::list<Plane*>::iterator it = planes.begin(); it != planes.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	planes.clear();

	if (!polyhedrons.empty()) {
		for (std::list<Polyhedron*>::iterator it = polyhedrons.begin(); it != polyhedrons.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	polyhedrons.clear();

	if (!triangles.empty()) {
		for (std::list<Triangle*>::iterator it = triangles.begin(); it != triangles.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	triangles.clear();

	if (!rays.empty()) {
		for (std::list<Ray*>::iterator it = rays.begin(); it != rays.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	rays.clear();

	// GRAPHICAL PRIMITIVES LIST CLEAN UP
	if (!g_primitives.empty()) {
		for (std::list<Primitive*>::iterator it = g_primitives.begin(); it != g_primitives.end(); it++) {
			if (it._Ptr->_Myval != nullptr) {
				delete it._Ptr->_Myval;
			}
		}
	}

	g_primitives.clear();

	
	LOG("Destroying 3D Physics simulation");
	/*
	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}
	for (list<btTypedConstraint*>::iterator it = constraints.begin(); it != constraints.end(); ++it) {
		world->removeConstraint(*it);
		delete (*it);
	}
	constraints.clear();

	for (list<btDefaultMotionState*>::iterator it = motions.begin(); it != motions.end(); ++it)
		delete (*it);
	motions.clear();

	for (list<btCollisionShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
		delete (*it);
	shapes.clear();

	for (list<PhysBody3D*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
		delete(*it);
	bodies.clear();

	for (list<PhysVehicle3D*>::iterator it = vehicles.begin(); it != vehicles.end(); it++)
		delete (*it);
	vehicles.clear();

	delete vehicle_raycaster;
	delete world;
	*/
	return true;
}

void ModulePhysics3D::Draw()
{
	for (std::list<Primitive*>::iterator it = g_primitives.begin(); it != g_primitives.end(); it++) {
		if ((*it) != nullptr) {
			if ((*it)->wire != App->renderer3D->enable_wireframe)
				(*it)->wire = App->renderer3D->enable_wireframe;
			if((*it)->visible)
				(*it)->Render();
		}
	}
}

Sphere * ModulePhysics3D::AddSphere(const float x, const float y, const float z, float radius, bool visible)
{
	math::float3 point = { x, y, z };
	bSphere* gs = new bSphere(radius);
	gs->SetPos(point.x, point.y, point.z);
	Sphere* s = new Sphere(point, radius);
	g_primitives.push_back(gs);
	spheres.push_back(s);

	App->imgui->ConsoleLog("Sphere Created");

	return s;
}

AABB * ModulePhysics3D::AddAABB(const vec center, const vec size, bool visible)
{
	AABB* ret = nullptr;

	ret = new AABB();

	ret->SetFromCenterAndSize(center, size);
	aabbs.push_back(ret);

	bCube* bAABB = new bCube(size.z, size.x, size.y);
	vec position;
	//position = ret->CornerPoint(2); // Corner (-1, +1, -1)
	position = ret->CenterPoint();
	bAABB->SetPos(position.x, position.y, position.z);
	bAABB->visible = visible;
	g_primitives.push_back(bAABB);

	App->imgui->ConsoleLog("AABB Created");

	return ret;
}

int ModulePhysics3D::CheckIntersec(Sphere * sp)
{
	int ret = 0;

	if (!spheres.empty()) {
		for (std::list<Sphere*>::iterator it = spheres.begin(); it != spheres.end(); it++) {
			if (it._Ptr->_Myval->Intersects(*sp)) {
				ret++;
			}
		}
	}

	return ret;
}

void ModulePhysics3D::SetToWire(bool flag)
{
	for (std::list<Primitive*>::iterator it = g_primitives.begin(); it != g_primitives.end(); it++) {
		if ((*it) != nullptr)
			(*it)->wire = flag;
	}
}

void ModulePhysics3D::CheckAllIntersec()
{
	int a_id = 1;
	for (std::list<Sphere*>::iterator it_a = spheres.begin(); it_a != spheres.end(); it_a++) {
		int b_id = 1;
		for (std::list<Sphere*>::iterator it_b = spheres.begin(); it_b != spheres.end(); it_b++) {
			if (it_a._Ptr->_Myval->Intersects(*(it_b._Ptr->_Myval))) {
				string collision_log = "Sphere " + std::to_string(a_id) + " collides with Sphere " + std::to_string(b_id);
				App->imgui->ConsoleLog(collision_log.c_str());
			}
			b_id++;
		}
		a_id++;
	}
}

// ---------------------------------------------------------

/*
PhysBody3D* ModulePhysics3D::AddBody(const Sphere& sphere, float mass)
{
	
	btCollisionShape* colShape = new btSphereShape(sphere.radius);
	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&sphere.transform);

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);
	
	return pbody;
}



// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cube& cube, float mass)
{
	btCollisionShape* colShape = new btBoxShape(btVector3(cube.size.x*0.5f, cube.size.y*0.5f, cube.size.z*0.5f));
	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cube.transform);

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysBody3D* ModulePhysics3D::AddBody(const Cylinder& cylinder, float mass)
{
	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder.height*0.5f, cylinder.radius, 0.0f));
	shapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&cylinder.transform);

	btVector3 localInertia(0, 0, 0);
	if(mass != 0.f)
		colShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody3D* pbody = new PhysBody3D(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodies.push_back(pbody);

	return pbody;
}

// ---------------------------------------------------------
PhysVehicle3D* ModulePhysics3D::AddVehicle(const VehicleInfo& info)
{
	btCompoundShape* comShape = new btCompoundShape();
	shapes.push_back(comShape);

	btCollisionShape* colShape = new btBoxShape(btVector3(info.chassis_size.x*0.5f, info.chassis_size.y*0.5f, info.chassis_size.z*0.5f));
	shapes.push_back(colShape);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z));

	comShape->addChildShape(trans, colShape);

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, 0);
	comShape->calculateLocalInertia(info.mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(info.mass, myMotionState, comShape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	body->setActivationState(DISABLE_DEACTIVATION);

	world->addRigidBody(body);

	btRaycastVehicle::btVehicleTuning tuning;
	tuning.m_frictionSlip = info.frictionSlip;
	tuning.m_maxSuspensionForce = info.maxSuspensionForce;
	tuning.m_maxSuspensionTravelCm = info.maxSuspensionTravelCm;
	tuning.m_suspensionCompression = info.suspensionCompression;
	tuning.m_suspensionDamping = info.suspensionDamping;
	tuning.m_suspensionStiffness = info.suspensionStiffness;

	btRaycastVehicle* vehicle = new btRaycastVehicle(tuning, body, vehicle_raycaster);

	vehicle->setCoordinateSystem(0, 1, 2);

	for(int i = 0; i < info.num_wheels; ++i)
	{
		btVector3 conn(info.wheels[i].connection.x, info.wheels[i].connection.y, info.wheels[i].connection.z);
		btVector3 dir(info.wheels[i].direction.x, info.wheels[i].direction.y, info.wheels[i].direction.z);
		btVector3 axis(info.wheels[i].axis.x, info.wheels[i].axis.y, info.wheels[i].axis.z);

		vehicle->addWheel(conn, dir, axis, info.wheels[i].suspensionRestLength, info.wheels[i].radius, tuning, info.wheels[i].front);
	}
	// ---------------------

	PhysVehicle3D* pvehicle = new PhysVehicle3D(body, vehicle, info);
	world->addVehicle(vehicle);
	vehicles.push_back(pvehicle);

	return pvehicle;
}

// ---------------------------------------------------------
void ModulePhysics3D::AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z), 
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	world->addConstraint(p2p);
	constraints.push_back(p2p);
	p2p->setDbgDrawSize(2.0f);
}

void ModulePhysics3D::AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisA, const vec3& axisB, bool disable_collision)
{
	btHingeConstraint* hinge = new btHingeConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z),
		btVector3(axisA.x, axisA.y, axisA.z), 
		btVector3(axisB.x, axisB.y, axisB.z));

	world->addConstraint(hinge, disable_collision);
	constraints.push_back(hinge);
	hinge->setDbgDrawSize(2.0f);
}

*/
// =============================================

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	//LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	//LOG("Bullet draw text: %s", textString);
	
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}
