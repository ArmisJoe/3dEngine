#pragma once
#ifndef __IMPORTERMESH_H__
#define __IMPORTERMESH_H__

#include "Importer.h"

class ImporterMesh : Importer {
public:
	ImporterMesh();
	~ImporterMesh();
public:
	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file);
};

#endif // !__IMPORTERMESH_H__
