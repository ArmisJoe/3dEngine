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
