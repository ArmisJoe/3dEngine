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
void AnimNode::CleanUp()
{

}

TransformKeys::VectorKey AnimNode::GetPosByTime(double time)
{
	TransformKeys::VectorKey ret;
	ret.time = time;
	ret.value = float3(0, 0, 0);

	if (NumPositionKeys() <= 0)
		return ret;

	ret = transKeys.positionKeys[0];
	for (int i = 0; i < NumPositionKeys(); i++) {
		if (transKeys.positionKeys[i].time <= time)
			if (transKeys.positionKeys[i].time > ret.time)
				ret = transKeys.positionKeys[i];
	}

	if (ret.time == time) // if it is already a setted frame
		return ret;
	else { // Interpolating
		TransformKeys::VectorKey from = ret;
		TransformKeys::VectorKey to = transKeys.positionKeys[0];

		for (int i = 0; i < NumPositionKeys(); i++) { // Settting [to] for the first time
			if (transKeys.positionKeys[i].time > from.time) {
				to = transKeys.positionKeys[i];
				break;
			}
		}

		for (int i = 0; i < NumPositionKeys(); i++) {
			if (transKeys.positionKeys[i].time > from.time)
				if (transKeys.positionKeys[i].time < to.time)
					to = transKeys.positionKeys[i];
		}

		if (from.time == to.time)
			ret = from;
		else
			ret = InterpolatePos(from, to, time);

	}

	return ret;
}

TransformKeys::QuatKey AnimNode::GetRotByTime(double time)
{
	TransformKeys::QuatKey ret;

	for (int i = 0; i < NumRotationKeys(); i++) {
		if (transKeys.rotationKeys[i].time == time) {
			ret = transKeys.rotationKeys[i];
			break;
		}
	}

	return ret;
}

TransformKeys::VectorKey AnimNode::GetScaByTime(double time)
{
	TransformKeys::VectorKey ret;

	for (int i = 0; i < NumScalingKeys(); i++) {
		if (transKeys.scalingKeys[i].time == time) {
			ret = transKeys.scalingKeys[i];
			break;
		}
	}

	return ret;
}

TransformKeys::VectorKey AnimNode::InterpolatePos(TransformKeys::VectorKey from, TransformKeys::VectorKey to, float time)
{
	TransformKeys::VectorKey ret;

	float timeA = from.time;
	float timeB = to.time;

	float diff_value = time - timeA;

	float interpolationWeight;
	if(timeA < timeB)
		interpolationWeight = diff_value / (timeB - timeA);
	else
		interpolationWeight = diff_value / (timeA - timeB);

	ret.time = time;
	ret.value = from.value.Lerp(to.value, interpolationWeight);

	return ret;
}

TransformKeys::QuatKey  AnimNode::InterpolateRot(TransformKeys::QuatKey * from, TransformKeys::QuatKey * to, float time)
{
	TransformKeys::QuatKey ret;
	return ret;
}

TransformKeys::VectorKey  AnimNode::InterpolateSca(TransformKeys::VectorKey * from, TransformKeys::VectorKey * to, float time)
{
	TransformKeys::VectorKey ret;
	return ret;
}
