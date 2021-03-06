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
	ComponentMesh* mesh = nullptr;
	void CollectGOs(GameObject* go, ResourceBone* b);
public:
	void	GetRootBone(GameObject*);
	GameObject*		GetRootBoneGO();

public:
	void GetGOFromBones();
	void LinkBonesToGO();
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
private:
	void MatchBoneAndGo(ResourceBone* bone, GameObject* go);
};