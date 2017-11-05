#pragma once
#ifndef __IMPORTERMATERIAL_H__
#define __IMPORTERMATERIAL_H__

#include "Importer.h"

class ImporterMaterial : public Importer {
public:
	ImporterMaterial();
	~ImporterMaterial();
public:
	bool Import(const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file);
	bool LoadCheckers(Texture* resource);
};

#endif // !__IMPORTERMATERIAL_H__
