#pragma once
#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Application.h"

class Importer {
public:
	Importer() {};
	virtual ~Importer() {};
public:
	virtual bool Import(const char* path, std::string& output_file) { return true; };
	virtual bool Import(const void* buffer, uint size, std::string& output_file) { return true; };
	virtual bool Load(const char* exported_file) { return true; };
public:
	Application* App;
};


#endif // !__MODULE_IMPORTER_H__
