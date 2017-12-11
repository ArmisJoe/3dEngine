 #include "ModuleTextures.h"
#include "ModuleAssimp.h"

#include "Application.h"

#include "glew-2.1.0\include\GL\glew.h"

#include "HelperFoos.h"

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

	clamping_str = "Clamp to Edge\0Repeat\0Mirrored Repeat\0";
	interpolate_str = "Nearest\0Linear\0";

	return true;
}

bool ModuleTextures::Start() {
	LoadConfig();
	return true;
}

update_status ModuleTextures::Update(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleTextures::CleanUp()
{


	return true;
}

void ModuleTextures::LoadConfig() {
	clamp_type = App->parson->config->GetNumber("configuration.textures.clamping");
	interpolation_type = App->parson->config->GetNumber("configuration.textures.interpolation");
}

Texture* ModuleTextures::LoadRawTexture(const char * path)
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

	return new_tex;
}

int ModuleTextures::ImportTexture(const char * path, std::string& output_file)
{
	bool ret = false;

	std::string checkPath = LIBRARY_TEXTURES;
	checkPath += GetFileFromPath(path);
	checkPath += ".dds";
	if (!App->fs->exists(checkPath))
		ret = Import(path, output_file);
	else {
		LOG("Texture already imported");
		output_file = checkPath;
		ret = -1;
	}

	if (ret == false)
		LOG("ERROR importing texture %s", path);

	return ret;
}

Texture * ModuleTextures::LoadDDSTexture(const char * path)
{
	Texture* tex = nullptr;

	tex = Load(path, tex);

	return tex;
}

Texture * ModuleTextures::LoadToDDS(const char * path, std::string& output_file)
{
	Texture* tex = nullptr;
	curr_tex_state = ImportTexture(path, output_file);
	switch (curr_tex_state) {
	case -1:	// Repeated Tex
		for (int i = 0; i < App->res->textures.size(); i++) {
			if (App->res->textures[i]->path == output_file) {
				tex = App->res->textures[i];
				break;
			}
		}
		break;
	case true:	// Success Import
		tex = LoadDDSTexture(output_file.c_str());
		break;
	default:   // Failed Import
		LOG("ERROR Importing texture %s", GetFileFromPath(path).c_str());
		break;
	}

	return tex;
}

Texture * ModuleTextures::LoadToDDS(const char * path)
{
	Texture* tex = nullptr;

	std::string output_file;

	if (ImportTexture(path, output_file)) {
		tex = LoadDDSTexture(output_file.c_str());
	}
	else {
		LOG("ERROR Importing texture %s", GetFileFromPath(path).c_str());
	}

	return tex;
}

void ModuleTextures::DrawConfigPanel()
{
	if (ImGui::CollapsingHeader("Textures")) 
	{
		if(clamping_str != nullptr)
			if (ImGui::Combo("Clamping Method", &clamp_type, clamping_str)) {
				App->parson->config->SetNumber("configuration.textures.clamping", clamp_type);
				App->parson->config->Save();
			}
		if(interpolate_str != nullptr)
			if (ImGui::Combo("Interpolation Method", &interpolation_type, interpolate_str)) {
				App->parson->config->SetNumber("configuration.textures.interpolation", interpolation_type);
				App->parson->config->Save();
			}
	}
}

bool ModuleTextures::Import(const char * path, std::string & output_file)
{
	bool ret = false;

	char* buffer = nullptr;
	uint size = App->fs->Load(path, &buffer);

	if (buffer != NULL) {
		ret = Import(buffer, size, output_file, GetFileFromPath(path).c_str());
	}

	if (buffer != nullptr) {
		mdelete[] buffer;
	}

	return ret;
}

bool ModuleTextures::Import(const void * buffer, uint size, std::string & output_file, const char * file_name = DEFAULT_TEXTURE_NAME)
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
					ret = App->fs->SaveUnique(LIBRARY_TEXTURES, (char*)data, file_name, "dds", ilsize, output_file, (file_name == DEFAULT_TEXTURE_NAME));
				}

				if (data != nullptr) {
					mdelete[] data;
				}

			}
			ilDeleteImages(1, &imageID);
		}
	}

	if (ret == false)
		LOG("ERROR importing texture from buffer");

	return ret;
}

Texture* ModuleTextures::Load(const char * exported_file)
{
	uint textureID = 0;
	ILuint imageID;

	Texture* new_tex = nullptr;

	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(exported_file)) {
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
			new_tex->path = exported_file;
			new_tex->name = GetFileFromPath(exported_file).c_str();

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

	LOG("Texture Load End:\n\t%s", exported_file);

	return new_tex;
}

Texture* ModuleTextures::Load(const char * exported_file, Texture * tex)
{
	char* buffer = nullptr;
	std::string file = exported_file;
	uint size = App->fs->Load(file.c_str(), &buffer);

	Texture* new_tex = nullptr;

	if (buffer != nullptr) {
		if (size > 0) {
			uint textureID = 0;
			ILuint imageID;
			ILenum error;

			ilGenImages(1, &imageID);
			ilBindImage(imageID);

			if (ilLoadL(IL_DDS, buffer, size)) {
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
					new_tex->path = exported_file;
					new_tex->name = GetFileFromPath(exported_file).c_str();

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
			ilDeleteImages(1, &imageID);
		}
	}
	
	if (buffer != nullptr)
		mdelete[] buffer;

	LOG("Texture Load End:\n\t%s", exported_file);

	return new_tex;
}

