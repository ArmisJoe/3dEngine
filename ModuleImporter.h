#pragma once
#ifndef __MODULE_IMPORTER_H__
#define __MODULE_IMPORTER_H__

#include "Module.h"

#include "glew-2.1.0\include\GL\glew.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")


class ModuleImporter : public Module {
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	virtual ~ModuleImporter();
public:
	virtual bool Import(const char* file, const char* path, std::string& output_file) = 0;
	virtual bool Import(const void* buffer, uint size, std::string& output_file) = 0;
	virtual bool Load(const char* exported_file, void* resource) = 0;
	
};

class TextureImporter : public ModuleImporter {
public:
	TextureImporter(Application* app, bool start_enabled = true);
	virtual ~TextureImporter();
public:
	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	
};

#endif // !__MODULE_IMPORTER_H__
