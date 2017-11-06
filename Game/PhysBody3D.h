#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include <list>

class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(const float &x,const float &y, const float &z) const;
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(const float &x, const float &y, const float &z);
	btRigidBody* GetBody() const {
		return body;
	}

	void SetAsSensor(bool is_sensor);

private:
	btRigidBody* body = nullptr;
	bool is_sensor = false;

public:
	std::list<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__