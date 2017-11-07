#pragma once
#ifndef __IMPORTERTEXTURE_H__
#define __IMPORTERTEXTURE_H__

#include "Importer.h"

class ImporterTexture {
public:
	ImporterTexture();
	virtual ~ImporterTexture();
public:
	bool Import(const char* path, std::string& output_file); // Creates a DDS File from other extensions [NOT LOADING FILE INTO ENGINE]
	bool Import(const void* buffer, uint size, std::string& output_file, const char* file_name); // Creates a DDS File from other extensions [NOT LOADING FILE INTO ENGINE]
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file);
	bool Load(const char* exported_file, Texture* tex); // fills 'tex' with the loaded texture
public:
	int clamp_type = 0;
	int interpolation_type = 0;
public:
	Application* App;
};

#endif // !__IMPORTERTEXTURE_H__
