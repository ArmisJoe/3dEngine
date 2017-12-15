#pragma once

#include "Component.h"
#include "ComponentMesh.h"
#include "ResourceBone.h"

class ComponentBone : public Component {
public:
	ComponentBone();
	ComponentBone(GameObject* argparent);
	ComponentBone(componentType argtype, GameObject* argparent);
	virtual ~ComponentBone();

public:
	void Start();
	void Update(float dt);
	void CleanUp();

	void Enable();
	void Disable();

	void OnEditor();
private:
	bool gos_filled = false;
	bool pairs_filled = false;
	ComponentMesh* mesh = nullptr;
	void CollectGOs(GameObject* go);
	bool insert_BoneToIterate(ResourceBone* bone);

public:
	ResourceBone*	GetRootBone() const;
	const GameObject*		GetRootBoneGO();

public:
	void GetGOFromBones();
	ComponentMesh* GetMesh() const;
	void SetMesh(ComponentMesh* m);
public:
	struct PairedGOToBones {
		string name;
		GameObject* object = nullptr;
		vector<ResourceBone*> bones;
	};

	std::vector<PairedGOToBones> pairedgotobones;

	std::vector<ResourceBone*> skeleton;

private:
	GameObject* rootBoneGO = nullptr;
};