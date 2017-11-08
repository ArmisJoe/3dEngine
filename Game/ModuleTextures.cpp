#include "ModuleTextures.h"
#include "ModuleAssimp.h"

#include "Application.h"

#include "glew-2.1.0\include\GL\glew.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ModuleTextures::ModuleTextures(bool start_enabled) : Module(start_enabled)
{
	name = "Texture";
}

ModuleTextures::~ModuleTextures()
{
	ilShutDown();
}

bool ModuleTextures::Init()
{
	// DeviL Initialization
	ilutRenderer(ILUT_OPENGL);
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	clamp_type = clampingTexType_ClampRepeat;
	interpolation_type = interpolationTexType_Linear;

	clamping_str = "Clamp to Edge\0Repeat\0Mirrored Repeat\0";
	interpolate_str = "Nearest\0Linear\0";

	return true;
}

bool ModuleTextures::CleanUp()
{
	return true;
}

Texture* ModuleTextures::LoadTexture(const char * path)
{
	uint textureID = 0;
	ILuint imageID;

	Texture* new_tex = nullptr;

	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(path)) {
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		//Flip the image if it is upside-down
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}
		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
			error = ilGetError();
			LOG("Texture Image conversion Failed: %d %s", error, iluErrorString(error));
		}
		else {	

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			new_tex = new Texture();

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			new_tex->id = textureID;
			new_tex->w = ilGetInteger(IL_IMAGE_WIDTH);
			new_tex->h = ilGetInteger(IL_IMAGE_HEIGHT);
			new_tex->format = ilGetInteger(IL_IMAGE_FORMAT);

			//Clamping Method
			GLint clampParam;
			switch (clamp_type) {
			case clampingTexType_ClampToEdge:
				clampParam = GL_CLAMP_TO_EDGE;
				break;
			case clampingTexType_ClampRepeat:
				clampParam = GL_REPEAT;
				break;
			case clampingTexType_ClampMirroredRepeat:
				clampParam = GL_MIRRORED_REPEAT;
				break;
			default:
				clampParam = GL_REPEAT;
				break;
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampParam);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampParam);
			// Interpolation Method
			GLint interParam;
			switch (interpolation_type) {
			case interpolationTexType_Nearest:
				interParam = GL_NEAREST;
				break;
			case interpolationTexType_Linear:
				interParam = GL_LINEAR;
				break;
			default:
				interParam = GL_LINEAR;
				break;
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interParam);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interParam);

			//Texture Specifications
			glTexImage2D(GL_TEXTURE_2D, 0, new_tex->format, new_tex->w, new_tex->h, 0, new_tex->format, GL_UNSIGNED_BYTE, ilGetData());

			if (new_tex != nullptr)
				new_tex->path = path;

			App->res->textures.push_back(new_tex);
			
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else {
		error = ilGetError();
		LOG("Image Load Error %d %s", error, iluErrorString(error));
	}

	//glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &imageID);

	LOG("Texture Load End:\n\t%s", path);

	return new_tex;
}

void ModuleTextures::DrawConfigPanel()
{
	if (ImGui::CollapsingHeader("textures")) 
	{
		if(clamping_str != nullptr)
			ImGui::Combo("Clamping Method", &clamp_type, clamping_str);
		if(interpolate_str != nullptr)
			ImGui::Combo("Interpolation Method", &interpolation_type, interpolate_str);
	}
}

