#pragma once
#ifndef __IMPORTERTEXTURE_H__
#define __IMPORTERTEXTURE_H__

#include "ModuleTextures.h"

#include "Importer.h"

class ImporterTexture : public Importer {
public:
	ImporterTexture();
	~ImporterTexture();
public:
	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file);
};

#endif // !__IMPORTERTEXTURE_H__
