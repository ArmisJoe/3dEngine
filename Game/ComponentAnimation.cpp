#include "ComponentAnimation.h"
#include "Application.h"

ComponentAnimation::ComponentAnimation() : Component(componentType_Animation)
{
	name = "Animation";
}

ComponentAnimation::ComponentAnimation(GameObject * argparent) : Component(componentType_Animation, argparent)
{
	name = "Animation";
}

ComponentAnimation::~ComponentAnimation()
{
}

void ComponentAnimation::Start()
{
}

void ComponentAnimation::Update(float dt)
{
	if (anim == nullptr || state == as_unloaded)
		return;

	time = App->game->GetGameTime();

	if (drawBones == true)
		DrawBones();

}

void ComponentAnimation::CleanUp()
{
}

bool ComponentAnimation::isPlay() const
{
	return state == as_play;
}

bool ComponentAnimation::isPause() const
{
	return state == as_pause;
}

void ComponentAnimation::DrawBones()
{
	if (anim == nullptr || state == as_unloaded)
		return;

	for (int i = 0; i < anim->NumChannels(); i++) {
		Bone* b = anim->Channels[i];
		TransformKeys::VectorKey* pos = nullptr;
		for (int n = 0; n < b->NumPositionKeys(); n++) {
			if (b->transKeys.positionKeys[n].time == time) {
				pos = &b->transKeys.positionKeys[n];
				break;
			}
		}
		if (pos == nullptr)
			return;

		bSphere s(0.5);
		s.color.Set(255, 211, 0);
		s.SetPos(pos->value.x, pos->value.y, pos->value.z);
		s.Render();
	}
}

void ComponentAnimation::Enable()
{
	isActive = true;
}

void ComponentAnimation::Disable()
{
	if (isPlay())
		Stop();
	isActive = false;
}

void ComponentAnimation::OnEditor()
{
	ImGui::Text("Animation Name: ");
	ImGui::Text("\t%s", (anim != nullptr) ? anim->name.c_str() : "");
}

void ComponentAnimation::Serialize(JSON_Doc * doc)
{
}

void ComponentAnimation::Load(JSON_Doc * doc)
{
}

void ComponentAnimation::Play()
{
}

void ComponentAnimation::Pause()
{
}

void ComponentAnimation::UnPause()
{
}

void ComponentAnimation::Stop()
{
}

void ComponentAnimation::Blend(Animation * next_anim, float time)
{
}
