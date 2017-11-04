#pragma once
#ifndef __MODULE_FILESYSTEM_H__
#define __MODULE_FILESYSTEM_H__

#include "Module.h"

#define LIBRARY_TEXTURES "Library/Textures"
#define LIBRARY_MATERIALS "Library/Materials"
#define LIBRARY_MESHES "Library/Meshes"

using namespace std;

class ModuleFileSystem : public Module {
public:
	ModuleFileSystem(Application* app, bool start_enabled = true);
	virtual ~ModuleFileSystem();
public:	
	bool Start();
	bool Update();
	bool CleanUp();

	string CreateFolder(const char* path, const char* name);
	void FileMove(const char* filepath, const char* new_path, bool replace_existing = true);
	void FileCopyPaste(const char* filepath, const char* new_path);
	void FileDelete(const char* filepath);
	bool SaveFile(const char* path, const char* file_content, const char* name, const char* extension, int size);
	bool SaveUnique(const char* path, const char* file_content, const char* name, const char* extension, int size, std::string& output_file); // returns new file path (IF it has been created successfully)
	bool SaveUnique(const char* path, const char* file_content, const char* name, const char* extension, int size);
	inline bool exists(const std::string& name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}
};

#endif // !__MODULE_FILESYSTEM_H__
