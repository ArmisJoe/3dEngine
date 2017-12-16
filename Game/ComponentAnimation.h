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

	void StartBlend(Animation* next_anim, float time);
	void ResetBlend();

	bool isPlay() const;
	bool isPause() const;
	void GetGOFromNodes();

	GameObject* GetRootBoneGO();

	void AddAnimation(Animation* anim);
	void ChangeAnimation(Animation* anim, float time = 0);

private:
	void DrawBones(GameObject* boneGO);
	GameObject* CheckBoneGoMatch(GameObject* go, AnimNode* b);
	void CollectGOs(AnimNode* node, GameObject* go);
public:
	Animation* anim = nullptr;
	AnimationState state = as_unloaded;

	std::vector<Animation*> animations;

	float time = 0.0f;

	bool drawBones = true;
	float boneDebugSize = 1.0f;

	bool loop = true;
	bool gos_filled = false;

private:
	// Blending
	bool blending = false;
	Animation* blend_second_anim = nullptr;
	Animation* blend_first_anim = nullptr;
	float blend_start_time = 0.0f;
	float blend_time = 0.0f;
	float blend_timer = 0.0f;

	// OnEditor/Config
	float conf_time_to_change = 0.5f;

	GameObject* rootBoneGO = nullptr;
};

#endif // !__COMPONENT_ANIMATION_H__
