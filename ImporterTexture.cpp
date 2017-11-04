#include "ImporterTexture.h"

#include "glew-2.1.0\include\GL\glew.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ImporterTexture::ImporterTexture()
{
}

ImporterTexture::~ImporterTexture()
{
}

bool ImporterTexture::Import(const char * file, const char * path, std::string & output_file)
{
	bool ret = false;

	//Getting Data


	// Data Save
	ILuint size;
	ILubyte* data = nullptr;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // Compression Mode
	size = ilSaveL(IL_DDS, NULL, 0); // Just getting Size of DDS Buffer
	if (size > 0) {
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0) { // Save Buffer
			ret = App->fs->SaveUnique(LIBRARY_TEXTURES, (char*)data, "texture", "dds", size, output_file);
		}
		if (data != nullptr) {
			delete[] data;
		}
	}


	return ret;
}

bool ImporterTexture::Import(const void * buffer, uint size, std::string & output_file)
{
	bool ret = false;

	return ret;
}

bool ImporterTexture::Load(const char * exported_file)
{
	bool ret = false;

	return ret;
}
