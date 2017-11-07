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

bool ImporterTexture::Import(const char * path, std::string & output_file)
{
	bool ret = false;

	char* buffer = nullptr;
	uint size = App->fs->Load(path, &buffer);

	if (buffer != NULL) {
		ret = Import(buffer, size, output_file, GetFileFromPath(path).c_str());
	}

	if (buffer != nullptr) {
		memdelete[] buffer;
	}

	return ret;
}

bool ImporterTexture::Import(const void * buffer, uint size, std::string & output_file, const char* file_name = "texture")
{
	bool ret = false;

	if (buffer != NULL) {
		ILuint imageID;
		ilGenImages(1, &imageID);
		ilBindImage(imageID);

		if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, size)) { // loads the buffer
			ilEnable(IL_FILE_OVERWRITE); // Enables File Overwriting (self-explanatory :s)

			ILuint ilsize;
			ILubyte* data;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // Compression mode [DXT]
			ilsize = ilSaveL(IL_DDS, NULL, 0); // Just setting new buffer size
			if (ilsize > 0) {
				data = new ILubyte[ilsize];
				if (ilSaveL(IL_DDS, data, ilsize) > 0) {
					ret = App->fs->SaveUnique(LIBRARY_TEXTURES, (char*)data, file_name, "dds", ilsize, output_file);
				}

				if (data != nullptr) {
					memdelete[] data;
				}

			}
			ilDeleteImages(1, &imageID);
		}
	}

	if (ret == false)
		LOG("ERROR importing texture from buffer");

	return ret;
}

bool ImporterTexture::Import(const void * buffer, uint size, std::string & output_file)
{
	bool ret = false;

	return ret;
}

bool ImporterTexture::Load(const char * path)
{
	bool ret = false;

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
			new_tex->path = path;
			new_tex->name = GetFileFromPath(path).c_str();
	
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

	return ret;
}

bool ImporterTexture::Load(const char * path, Texture * tex)
{
	bool ret = false;

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
			new_tex->path = path;
			new_tex->name = GetFileFromPath(path).c_str();

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

			tex = new_tex;
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

	return ret;
};
