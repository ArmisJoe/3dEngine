#pragma once
#ifndef __COMPONENT_ANIMATION_H__
#define __COMPONENT_ANIMAYION_H__

#include "Component.h"
#include "ModuleAnimationLoader.h"

enum AnimationState {
	as_play = 0,
	as_pause,
	as_stop,
	as_unloaded,
	// Unknown always last
	as_unknown
};

class ComponentAnimation : public Component {
public:
	ComponentAnimation();
	ComponentAnimation(GameObject* argparent);
	~ComponentAnimation();
public: //Component::
	void Start();
	void Update(float dt);
	void CleanUp();

	void Enable();
	void Disable();

	void OnEditor();

	void Serialize(JSON_Doc* doc);
	void Load(JSON_Doc* doc);

public: // ComponentAnimation::
	void Play();
	void Pause();
	void UnPause();
	void Stop();
	void Blend(Animation* next_anim, float time);

	bool isPlay() const;
	bool isPause() const;
	void GetGOFromNodes();
private:
	void DrawBones(GameObject* boneGO);
	GameObject* CheckBoneGoMatch(GameObject* go, AnimNode* b);
	void CollectGOs(AnimNode* node, GameObject* go);
public:
	Animation* anim = nullptr;
	AnimationState state = as_unloaded;

	float time = 0.0f;

	bool drawBones = true;
	float boneDebugSize = 1.0f;

	bool loop = true;
	bool gos_filled = false;
};

#endif // !__COMPONENT_ANIMATION_H__
