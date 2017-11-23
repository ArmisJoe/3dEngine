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
	std::string name;
	int duration = 0;
	int tickspersec = 0;
	std::vector<Bone*> Channels; // Bones

public:
	float DurationSec() {
		return (float)(duration / tickspersec);
	};
};

class Bone {
public:
	Bone() {};
	virtual ~Bone() {};
public:
	std::string name;
	TransformKeys transKeys;
};

struct TransformKeys {
	double Time = 0;
	float3 translationKey;
	float3 scaleKey;
	Quat rotationKey;
};

#endif // !__ANIMATION_H__
