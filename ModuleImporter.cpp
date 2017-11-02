#include "ModuleImporter.h"
#include "Application.h"

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Importer";
}

ModuleImporter::~ModuleImporter()
{
}

TextureImporter::TextureImporter(Application * app, bool start_enabled) : ModuleImporter(app, start_enabled)
{
}

TextureImporter::~TextureImporter()
{
}

bool TextureImporter::Import(const char * file, const char * path, std::string & output_file)
{
	bool ret = false;


	return ret;
}

bool TextureImporter::Import(const void * buffer, uint size, std::string & output_file)
{
	bool ret = false;
	//
	//ILuint size;
	//ILubyte * data;
	//
	//ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	//size = ilSaveL(IL_DDS, NULL, 0); 
	//if (size > 0) {
	//	data = new ILubyte[size]; 
	//	if (ilSaveL(IL_DDS, data, size) > 0)
	//		ret = App->fs->SaveUnique(output_file, data, size, LIBRARY_TEXTURES_FOLDER, "texture", "dds");
	//	RELEASE_ARRAY(data);
	//}

	return ret;
}
