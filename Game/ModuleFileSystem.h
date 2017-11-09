#pragma once
#ifndef __MODULE_FILESYSTEM_H__
#define __MODULE_FILESYSTEM_H__

#include "Module.h"

#define LIBRARY_TEXTURES "Library/Textures/"
#define LIBRARY_MATERIALS "Library/Materials/"
#define LIBRARY_MESHES "Library/Meshes/"

#define SETTINGS_DIRECTORY "Settings/"

using namespace std;

enum ExtensionType {
	et_texture = 0,
	et_geometry,
	// unknown always last
	et_unknown
};

class ModuleFileSystem : public Module {
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	virtual ~ModuleFileSystem();
public:	
	bool Init();
	bool Start();

	bool CleanUp();

	std::string CreateFolder(const char* path, const char* name);
	void FileMove(const char* filepath, const char* new_path, bool replace_existing = true);
	void FileCopyPaste(const char* filepath, const char* new_path);
	void FileDelete(const char* filepath);
	bool SaveFile(const char* path, const char* file_content, const char* name, const char* extension, int size);
	bool SaveUnique(const char* path, const char* file_content, const char* name, const char* extension, int size, std::string& output_file, bool gen_uid = true); // returns new file path. if gen_uid == false -> will not save again repeated files
	bool SaveUnique(const char* path, const char* file_content, const char* name, const char* extension, int size, bool gen_uid = true); // if gen_uid == false -> will not save again repeated files
	inline bool exists(const std::string& path) {
		struct stat buffer;
		return (stat(path.c_str(), &buffer) == 0);
	}
	uint Load(const char* path, char** buffer);

	int GetTypeFromExtension(mutable const char* ext);

};

#endif // !__MODULE_FILESYSTEM_H__
