/*#include "ModuleImGui.h"

ModuleImGui::ModuleImGui(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleImGui::~ModuleImGui()
{

}

bool ModuleImGui::Init()
{
	
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;

	io.RenderDrawListsFn = NULL;

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	/*
	// TODO: At this points you've got the texture data and you need to upload that your your graphic system:
	MyTexture* texture = MyEngine::CreateTextureFromMemoryPixels(pixels, width, height, TEXTURE_TYPE_RGBA)
		// TODO: Store your texture pointer/identifier (whatever your engine uses) in 'io.Fonts->TexID'. This will be passed back to your via the renderer.
		io.Fonts->TexID = (void*)texture;
	*/
/*
	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGuiIO& io = ImGui::GetIO();

	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(App->input->GetMouseX(), App->input->GetMouseY());

	ImGui::NewFrame();

	//Code Here

	// !-- Code Here

	ImGui::Render();

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	return true;
}
*/