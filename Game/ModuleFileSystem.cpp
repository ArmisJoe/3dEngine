#include "ModuleFileSystem.h"
#include "Application.h"
#include <fstream>

#include "HelperFoos.h"

ModuleFileSystem::ModuleFileSystem(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "FileSystem";
}

ModuleFileSystem::~ModuleFileSystem()
{
	name = "FileSystem";
}

bool ModuleFileSystem::Init()
{
	
	return true;
}

bool ModuleFileSystem::Start()
{
	// Settings
	CreateFolder("", "Settings");
	// Libraries
	CreateFolder("", "Library");
	CreateFolder("Library", "Textures");
	CreateFolder("Library", "Meshes");
	CreateFolder("Library", "Materials");
	// Assets
	CreateFolder("", "Assets");

	return true;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}

string ModuleFileSystem::CreateFolder(const char * path, const char * name)
{
	string ret;

	string strPath = path;
	if (strPath.length() > 0 && strPath[strPath.length() - 1] != '\\') {
		strPath += '\\';
	}
	strPath += name;

	if (CreateDirectory(strPath.c_str(), NULL) == 0) {
		LOG("ERROR Creating Directory %s [%s]", name, strerror(errno));
		return ret;
	}

	ret = strPath + '\\';

	return ret;
}

void ModuleFileSystem::FileMove(const char * filepath, const char * new_path, bool replace_existing)
{
	string path = new_path;
	if (path[path.length() - 1] != '\\') {
		path += '\\';
	}
	path += GetFileFromPath(filepath);

	if (!replace_existing) {
		if (MoveFile(filepath, path.c_str())) {
			LOG("Error with MoveFile:\n\t%s", filepath);
		}
	}
	else {
		if (MoveFileEx(filepath, path.c_str(), MOVEFILE_REPLACE_EXISTING)) {
			LOG("ERROR MoveFile:\n\t%s", filepath);
		}
	}

}

void ModuleFileSystem::FileCopyPaste(const char * filepath, const char * new_path)
{
	string path = new_path;
	if (path[path.length() - 1] != '\\') {
		path += '\\';
	}
	path += GetFileFromPath(filepath);
	if (CopyFile(filepath, path.c_str(), false)) {
		LOG("ERROR Coping File");
	}
}

void ModuleFileSystem::FileDelete(const char * filepath)
{
	if (DeleteFile(filepath) == 0) {
		DWORD error = GetLastError();
		if (error == ERROR_FILE_NOT_FOUND) {
			LOG("ERROR Deleting file %d", error);
		}
	}
}

bool ModuleFileSystem::SaveFile(const char * path, const char * file_content, const char * name, const char * extension, int size)
{
	bool ret = false;

	string file = path;
	file += name;
	file += ".";
	file += extension;

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else
	{
		LOG("ERROR saving file %s: ", name);
	}

	fclose(new_file);
	return ret;
}

bool ModuleFileSystem::SaveUnique(const char * path, const char * file_content, const char * name, const char * extension, int size, bool gen_uid)
{
	bool ret = false;

	uint uniqueID = 0;
	string file;

	if (gen_uid == true) {
		do {
			file = path;
			file += name;
			file += "_";
			file += std::to_string(uniqueID++);
			file += ".";
			file += extension;
		} while (exists(file));
	}
	else {
		file = path;
		file += name;
		file += ".";
		file += extension;
	}

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else
	{
		LOG("ERROR saving unique file %s: ", name);
	}

	fclose(new_file);
	return ret;
}

bool ModuleFileSystem::SaveUnique(const char * path, const char * file_content, const char * name, const char * extension, int size, std::string& output_file, bool gen_uid)
{
	bool ret = false;

	uint uniqueID = 0;
	string file;

	if (gen_uid == true) {
		do {
			file = path;
			file += name;
			file += "_";
			file += std::to_string(uniqueID++);
			file += ".";
			file += extension;
		} while (exists(file));
	}
	else {
		file = path;
		file += name;
		file += ".";
		file += extension;
	}

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file != nullptr)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		output_file = file;
		ret = true;
		fclose(new_file);
	}
	else
	{
		LOG("ERROR unique saving file:\n\t%s\n\tERROR: %s ", name, strerror(errno));
	}

	return ret;
}


uint ModuleFileSystem::Load(const char * path, char ** buffer)
{
	uint ret = 0;

	std::ofstream;
	FILE* load_file = fopen(path, "rb");

	if (load_file) {
		
		int size = 0;

		std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
		size = in.tellg();

		if (size > 0) {
			*buffer = new char[size];
			uint readed = (uint)fread(*buffer, sizeof(char), size, load_file);
			if (readed != size) {
				LOG("ERROR while reading file:\n\t%s", path);
				if (*buffer != nullptr)
					mdelete[] *buffer;
			}
			else {
				ret = readed;
			}
		}
		if (fclose(load_file) != 0) {
			LOG("ERROR while closing file:\n\t%s", path);
		}
	}
	else {
		LOG("ERROR while opening file:\n\t%s\n\t%s", path, strerror(errno));
	}

	return ret;
}

int ModuleFileSystem::GetTypeFromExtension(const char * ext)
{
	int ret = et_unknown;

	if (strcmp(ext, "DDS") == 0 || strcmp(ext, "dds") == 0)
		ret = et_texture;
	if (strcmp(ext, "TGA") == 0 || strcmp(ext, "tga") == 0)
		ret = et_texture;
	if (strcmp(ext, "JPG") == 0 || strcmp(ext, "jpg") == 0)
		ret = et_texture;
	if (strcmp(ext, "PNG") == 0 || strcmp(ext, "png") == 0)
		ret = et_texture;
	if (strcmp(ext, "FBX") == 0 || strcmp(ext, "fbx") == 0)
		ret = et_geometry;

	return ret;
}
