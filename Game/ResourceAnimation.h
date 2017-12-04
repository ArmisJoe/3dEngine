#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <string>
#include <vector>

#include "MathGeoLib\MathGeoLib.h"

struct Bone;

class Animation {
public:
	Animation();
	virtual ~Animation();
public:
	void CleanUp();
public:
	std::string name;
	uint duration = 0;
	uint tickspersec = 0;
	std::vector<Bone*> Channels; // Bones

public: // Utilities
	float DurationSec() const {
		return (float)(duration / tickspersec);
	};

	uint NumChannels() const {
		return Channels.size();
	}
};

class Bone {
public:
	Bone() {};
	virtual ~Bone() {};
public:
	std::string name;
	TransformKeys transKeys;
public:
	void CleanUp();

public: // Utilities
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

#endif // !__ANIMATION_H__
