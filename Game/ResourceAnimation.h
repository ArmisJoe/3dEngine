#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <string>
#include <vector>

#include "MathGeoLib\MathGeoLib.h"
#include "ComponentMesh.h"

struct AnimNode;

struct TransformKeys;

class Animation {
public:
	Animation();
	virtual ~Animation();
public:
	void CleanUp();
public:
	std::string name;
	unsigned int duration = 0;
	unsigned int tickspersec = 0;
  
	std::vector<AnimNode*> Channels; // Bones
  
	// Serialize
	std::string path;
	std::string rawpath;

public: // Utilities
	AnimNode* GetRootBone() {
		return (Channels.empty() ? nullptr : Channels[0]);
	}
	float DurationSec() const {
		return (float)(duration / tickspersec);
	};

	unsigned int NumChannels() const {
		return Channels.size();
	}
};

struct TransformKeys {
	// Key Declarations
	struct VectorKey {
		double time = 0;
		float3 value;
	};
	struct QuatKey {
		double time = 0;
		Quat value;
	};

	// Values
	std::vector<VectorKey> positionKeys;
	std::vector<QuatKey> rotationKeys;
	std::vector<VectorKey> scalingKeys;
};

class AnimNode {
public:
	AnimNode() {};
	virtual ~AnimNode() {};
public:
	std::string name;
	TransformKeys transKeys;
	GameObject* object = nullptr;
public:
	void CleanUp();

public: // Bone::
	TransformKeys::VectorKey GetPosByTime(double time);
	TransformKeys::QuatKey GetRotByTime(double time);
	TransformKeys::VectorKey GetScaByTime(double time);

public: // Utilities

	TransformKeys::VectorKey InterpolatePos(TransformKeys::VectorKey from, TransformKeys::VectorKey to, float time);
	TransformKeys::QuatKey InterpolateRot(TransformKeys::QuatKey from, TransformKeys::QuatKey to, float time);
	TransformKeys::VectorKey InterpolateSca(TransformKeys::VectorKey from, TransformKeys::VectorKey to, float time);

	int NumPositionKeys() const {
		return transKeys.positionKeys.size();
	}
	int NumRotationKeys() const {
		return transKeys.rotationKeys.size();
	}
	int NumScalingKeys() const {
		return transKeys.scalingKeys.size();
	}
};


#endif // !__ANIMATION_H__
