#include "ModuleTextures.h"
#include "ModuleAssimp.h"

#include "glew-2.1.0\include\GL\glew.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ModuleTextures::ModuleTextures(Application * app, bool start_enabled) : Module(app, start_enabled)
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

	return true;
}

bool ModuleTextures::CleanUp()
{
	while (!textures.empty())
	{
		delete[] textures.front();
		textures.pop_front();
	}
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
			//LOG("Texture Image conversion Failed: %d %s", error, iluErrorString(error));
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Interpolation Method
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//Texture Specifications
			glTexImage2D(GL_TEXTURE_2D, 0, new_tex->format, new_tex->w, new_tex->h, 0, new_tex->format, GL_UNSIGNED_BYTE, ilGetData());

			textures.push_back(new_tex);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else {
		error = ilGetError();
		//LOG("Image Load Error %d %s", error, iluErrorString(error));
	}

	//glBindTexture(GL_TEXTURE_2D, 0);

	ilDeleteImages(1, &imageID);

	//LOG("Texture Loading Finished");

	return new_tex;
}

