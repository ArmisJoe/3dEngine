#include "ModuleFileSystem.h"
#include "Application.h"
#include <fstream>

ModuleFileSystem::ModuleFileSystem(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Start()
{
	return true;
}

bool ModuleFileSystem::Update()
{
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
	if (strPath[strPath.length() - 1] != '\\') {
		strPath += '\\';
	}
	strPath += name;
	DWORD error = GetLastError();

	if (CreateDirectory(strPath.c_str(), NULL) == 0) {
		error = GetLastError();
		LOG("ERROR Creating Directory %s :: %d", path, error);
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
		LOG("Error saving file %s: ", name);
	}

	fclose(new_file);
	return ret;
}

bool ModuleFileSystem::SaveUnique(const char * path, const char * file_content, const char * name, const char * extension, int size)
{
	bool ret = false;

	uint uniqueID = 0;
	string file;

	do {
		file = path;
		file += name;
		file += "_";
		file += uniqueID++;
		file += ".";
		file += extension;
	} while (exists(file));


	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else
	{
		LOG("Error saving file %s: ", name);
	}

	fclose(new_file);
	return ret;
}

bool ModuleFileSystem::SaveUnique(const char * path, const char * file_content, const char * name, const char * extension, int size, std::string& output_file)
{
	bool ret = false;

	uint uniqueID = 0;
	string file;

	do {
		file = path;
		file += name;
		file += "_";
		file += uniqueID++;
		file += ".";
		file += extension;
	} while (exists(file));

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		output_file = file;
		ret = true;
	}
	else
	{
		LOG("Error saving file %s: ", name);
	}

	fclose(new_file);
	return ret;
}


uint ModuleFileSystem::Load(const char * path, char ** buffer)
{
	uint ret = 0;

	std::ofstream;
	FILE* load_file = fopen(path, "r");

	if (load_file != nullptr) {
		fseek(load_file, 0, SEEK_END); // pointer to end
		int size = ftell(load_file); // Tell the pointer pos a.k.a 'file size'
		fseek(load_file, 0, SEEK_SET); // return ptr to begining

		if (size > 0) {
			*buffer = new char[size];
			uint readed = (uint)fread(*buffer, sizeof(char), size, load_file);
			if (readed != size) {
				LOG("ERROR while reading file %s", path);
				if (*buffer != nullptr)
					memdelete[] *buffer;
			}
			else {
				ret = readed;
			}
		}
		if (fclose(load_file) == 0) {
			LOG("ERROR while closing file %s", path);
		}
	}
	else {
		LOG("ERROR while opening file %s", path);
	}

	return ret;
}
