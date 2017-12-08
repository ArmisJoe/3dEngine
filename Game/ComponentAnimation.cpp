#include "ComponentAnimation.h"
#include "Application.h"
#include "ComponentTransform.h"

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
	if (anim == nullptr) {
		state = as_unloaded;
		return;
	}
	
	if (App->game->GetGameState() == GameState::gameState_play)
		state = as_play;
	if (App->game->GetPaused() == true)
		state = as_pause;

	// Time Interactions
	switch (state) {
	case as_play:
		time += (anim->tickspersec * dt) * App->game->GetTimeMultiplier(); // Ticks/sec * sec [* xTime] = ticks
		break;
	case as_pause:
		time = time;
		break;
	case as_stop:
		time = 0;
		break;
	case as_unloaded:
		break;
	default:
		LOG("Unknown State -- going back to [Stop] state");
		state = as_stop;
		time = 0;
		break;
	}

	// Looping
	if (loop == true) {
		if (time > anim->duration)
			time -= anim->duration;
	}

	// Bone Movement
	if (this->HasParent()) {
		for (int i = 0; i < anim->NumChannels(); i++) {
			AnimNode* b = anim->Channels[i];
			GameObject* targetGo = CheckBoneGoMatch(this->parent, b);
			if (targetGo != nullptr) { // Now we have the Go ('BoneGo') that will move
				ComponentTransform* trans = (ComponentTransform*)targetGo->FindComponent(componentType_Transform);
				if (trans != nullptr) {
					TransformKeys::VectorKey* bPos = b->GetPosByTime(this->time);
					TransformKeys::QuatKey* bRot = b->GetRotByTime(this->time);
					TransformKeys::VectorKey* bSca = b->GetScaByTime(this->time);
					// NO INTERPOLATION
					if (bPos != nullptr) {
						trans->ChangeLocalPosition(bPos->value);
					}
					if (bRot != nullptr) {
						Quat newQ;
						newQ.x = bRot->value.x * RADTODEG;
						newQ.y = bRot->value.y * RADTODEG;
						newQ.z = bRot->value.z * RADTODEG;
						newQ.w = bRot->value.w * RADTODEG;
						trans->ChangeLocalRotation(newQ);
					}
					if (bSca != nullptr) {
						trans->ChangeLocalScale(bSca->value);
					}

				}
			}
		}
	}
	else {
		LOG("ERROR No Parent Animation");
	}

	// Bone Debug Render
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
	if (anim == nullptr)
		return;

	for (int i = 0; i < anim->NumChannels(); i++) {
		AnimNode* b = anim->Channels[i];
		TransformKeys::VectorKey* pos = nullptr;
		for (int n = 0; n < b->NumPositionKeys(); n++) {
			if (b->transKeys.positionKeys[n].time == time) {
				pos = &b->transKeys.positionKeys[n];
				break;
			}
		}
		if (pos == nullptr)
			return;

		bSphere s(100);
		s.color.Set(255, 211, 0);
		s.SetPos(pos->value.x, pos->value.y, pos->value.z);
		s.Render();
	}
}

GameObject* ComponentAnimation::CheckBoneGoMatch(GameObject* go, AnimNode * b)
{
	GameObject* ret = nullptr;
	
	if (go == nullptr || b == nullptr)
		return nullptr;

	if (go->GetName() == b->name)
		return go;
	
	for (int i = 0; i < go->children.size(); i++) {
		ret = CheckBoneGoMatch(go->children[i], b);
		if (ret != nullptr)
			break;
	}

	return ret;
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
	ImGui::TextColored(COLOR_YELLOW, "Animation Name: ");
	ImGui::Text("\t%s", (anim != nullptr) ? anim->name.c_str() : "");
	ImGui::TextColored(COLOR_YELLOW, "Time: ");
	ImGui::SliderFloat("Frames", &time, 0, anim->duration, "%.0f", 1.0f);
	ImGui::Checkbox("Loop?", &loop);
}

void ComponentAnimation::Serialize(JSON_Doc * doc)
{
}

void ComponentAnimation::Load(JSON_Doc * doc)
{
}

void ComponentAnimation::Play()
{
	if(state == as_stop || state == as_pause)
		state = as_play;
}

void ComponentAnimation::Pause()
{
	if(state == as_play)
		state = as_pause;
}

void ComponentAnimation::UnPause()
{
	if(state == as_pause)
		state = as_play;
}

void ComponentAnimation::Stop()
{
	state = as_stop;
}

void ComponentAnimation::Blend(Animation * next_anim, float time)
{
}
