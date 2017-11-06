#pragma once
#ifndef __MODULE_FILESYSTEM_H__
#define __MODULE_FILESYSTEM_H__

#include "Module.h"

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

public:
	string asset_path;
	string library_path;
	string mesh_lib_path;
	string texture_lib_path;
};

#endif // !__MODULE_FILESYSTEM_H__
