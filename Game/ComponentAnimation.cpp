#include "ComponentAnimation.h"
#include "Application.h"
#include "ComponentTransform.h"

ComponentAnimation::ComponentAnimation() : Component(componentType_Animation)
{
	typeName = "Animation";
	name = "Animation";
}

ComponentAnimation::ComponentAnimation(GameObject * argparent) : Component(componentType_Animation, argparent)
{
	typeName = "Animation";
	name = "Animation";
}

ComponentAnimation::~ComponentAnimation()
{
}

void ComponentAnimation::Start()
{
	anim = (!animations.empty()) ? animations[0] : nullptr;
	GetGOFromNodes();
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
	else
		if (time > anim->duration)
			time = anim->duration;

	// Bone Movement
	if (this->HasParent()) {
		for (int i = 0; i < anim->NumChannels(); i++) {
			AnimNode* b = anim->Channels[i];
			GameObject* targetGo = nullptr;
			GetGOFromNodes();
			targetGo = b->object;
			//GameObject* targetGo = CheckBoneGoMatch(this->parent, b);
			if (targetGo != nullptr) { // Now we have the Go ('BoneGo') that will move
				// Bone Debug Render
				if (drawBones == true)
					DrawBones(targetGo);
				ComponentTransform* trans = (ComponentTransform*)targetGo->FindComponent(componentType_Transform);
				if (trans != nullptr) {
					TransformKeys::VectorKey bPos;
					TransformKeys::QuatKey bRot;
					TransformKeys::VectorKey bSca;

					if (blending) { // Blending
						blend_timer += dt;

						AnimNode* blending_b = nullptr;
						for (int k = 0; k < blend_second_anim->NumChannels(); k++) {
							if (b->name == blend_second_anim->Channels[k]->name) {
								blending_b = blend_second_anim->Channels[k];
								break;
							}
						}

						if (blending_b != nullptr) {
							bPos = b->InterpolatePos(b->GetPosByTime(blend_start_time), blending_b->GetPosByTime(0), blend_time);
							bRot = b->InterpolateRot(b->GetRotByTime(blend_start_time), blending_b->GetRotByTime(0), blend_time);
							bSca = b->InterpolateSca(b->GetScaByTime(blend_start_time), blending_b->GetScaByTime(0), blend_time);
						}

						if (blend_timer >= blend_time) {
							anim = blend_second_anim;
							ResetBlend();
						}
					}
					else {	// No Blending
						bPos = b->GetPosByTime(this->time);
						bRot = b->GetRotByTime(this->time);
						bSca = b->GetScaByTime(this->time);
					}

					trans->SetPosition(bPos.value);
					trans->SetQuatRotation(bRot.value);
					trans->SetScale(bSca.value);
				}
			}
		}
	}
	else {
		LOG("ERROR No Parent Animation");
	}


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

void ComponentAnimation::GetGOFromNodes()
{
	if (gos_filled == false)
	{
		for (int i = 0; i < anim->NumChannels(); ++i)
		{
			CollectGOs(anim->Channels[i], this->parent);
		}
		gos_filled = true;
	}
}

GameObject * ComponentAnimation::GetRootBoneGO()
{
	GameObject* ret = nullptr;

	if (rootBoneGO != nullptr)
		return rootBoneGO;

	if (GetParent() == nullptr || anim == nullptr || anim->GetRootBone() == nullptr)
		return nullptr;

	for (int i = 0; i < GetParent()->children.size(); i++) {
		if (GetParent()->children[i]->GetName() == anim->GetRootBone()->name) {
			rootBoneGO = GetParent()->children[i];
			return rootBoneGO;
		}
	}

	return ret;
}

void ComponentAnimation::AddAnimation(Animation * anim)
{
	animations.push_back(anim);
}

void ComponentAnimation::ChangeAnimation(Animation * anim, float time)
{
	StartBlend(anim, time);
}

void ComponentAnimation::DrawBones(GameObject* boneGO)
{
	if (anim == nullptr)
		return;
	ComponentTransform* t = (ComponentTransform*)boneGO->FindComponent(componentType_Transform);
	if (t == nullptr)
		return;

		//App->renderer3D->debugger->DrawAABB(boneGO->GetTransform()->GetGlobalPosition(), float3(0.1, 0.1, 0.1), float3(255, 0, 0));
}


void ComponentAnimation::CollectGOs(AnimNode * node, GameObject * go)
{
	if (node == nullptr || go == nullptr)
		return;

	if (go->GetName() == node->name)
	{
		node->object = go;
		return;
	}

	for (uint i = 0; i < go->children.size(); ++i)
	{
		CollectGOs(node, go->children[i]);
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
	ImGui::TextColored(COLOR_YELLOW, "Current Animation: ");
	ImGui::Text("\t%s", (anim != nullptr) ? anim->name.c_str() : "");
	ImGui::TextColored(COLOR_YELLOW, "Time: ");
	ImGui::SliderFloat("Frames", &time, 0, anim->duration, "%.0f", 1.0f);
	ImGui::Checkbox("Loop?", &loop);
	if (ImGui::CollapsingHeader("Animations")) {
		for (int i = 0; i < animations.size(); i++) {
			Animation* a = animations[i];
			ImGui::Text("%s", a->name.c_str());
			ImGui::SameLine();
			bool selected = (a == anim);
			if (ImGui::Checkbox("", &selected)) {
				ChangeAnimation(a, conf_time_to_change);
			}
			ImGui::SameLine();
			ImGui::InputFloat("Time", &conf_time_to_change, 0.1f, 0.1f);
			ImGui::Separator();
		}
	}
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

void ComponentAnimation::StartBlend(Animation * next_anim, float time)
{
	blend_first_anim = this->anim;
	blend_start_time = this->time;
	blend_second_anim = next_anim;
	blend_time = time;
	blend_timer = 0.0f;
	blending = true;
}

void ComponentAnimation::ResetBlend()
{
	blend_first_anim = nullptr;
	blend_second_anim = nullptr;
	blend_time = 0.0f;
	blending = false;
}
