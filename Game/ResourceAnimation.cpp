#include "ResourceAnimation.h"

#include "mmgr\mmgr.h"

// [ANIMATION]
Animation::Animation()
{
}

Animation::~Animation()
{
}
void Animation::CleanUp()
{
	for (int i = 0; i < NumChannels(); i++) {
		if (Channels[i] != nullptr) {
			Channels[i]->CleanUp();
			mdelete Channels[i];
		}
	}
	Channels.clear();
}

// [BONE]
void Bone::CleanUp()
{

}

TransformKeys::VectorKey * Bone::GetPosByTime(double time)
{
	TransformKeys::VectorKey* ret = nullptr;

	for (int i = 0; i < NumPositionKeys(); i++) {
		if (transKeys.positionKeys[i].time == time) {
			ret = &transKeys.positionKeys[i];
			break;
		}
	}

	return ret;
}

TransformKeys::QuatKey * Bone::GetRotByTime(double time)
{
	TransformKeys::QuatKey* ret = nullptr;

	for (int i = 0; i < NumRotationKeys(); i++) {
		if (transKeys.rotationKeys[i].time == time) {
			ret = &transKeys.rotationKeys[i];
			break;
		}
	}

	return ret;
}

TransformKeys::VectorKey * Bone::GetScaByTime(double time)
{
	TransformKeys::VectorKey* ret = nullptr;

	for (int i = 0; i < NumScalingKeys(); i++) {
		if (transKeys.scalingKeys[i].time == time) {
			ret = &transKeys.scalingKeys[i];
			break;
		}
	}

	return ret;
}
