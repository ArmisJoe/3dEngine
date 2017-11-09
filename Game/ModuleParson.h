#pragma once

#ifndef __MODULE_PARSON_H__
#define __MODULE_PARSON_H__

#include "Module.h"
#include "parson.h"

class JSON_Doc
{
public:
	JSON_Doc(JSON_Value* value, JSON_Object* object, const char* path);
	~JSON_Doc();

	void SetString(const char* set, const char* str);
	void SetBool(const char* set, bool bo);
	void SetNumber(const char* set, double nu);
	void SetArray(const char* set);
	const int GetArraySize(const char* arr);
	const char* GetStringArr(const char* arr, int idx);
	const bool GetBoolArr(const char* arr, int idx);
	const double GetNumberArr(const char* arr, int idx);
	void AddStringArr(const char* arr, const char* str);
	void AddBoolArr(const char* arr, bool bo);
	void AddNumberArr(const char* arr, double nu);
	bool FindArrayValue(const char* arr, int idx, json_value_type type);
	const char* GetString(const char* str);
	bool GetBool(const char* bo);
	double GetNumber(const char* nu);

	const char* GetPath();
	void Save();
	void CleanUp();

private:
	JSON_Value*  value = nullptr;
	JSON_Object* object = nullptr;
	const char*  path;

};


class ModuleParson : public Module {
public:
	ModuleParson(Application* app, bool start_enabled = true);
	~ModuleParson();

	bool Init();
	bool CleanUp();

	JSON_Doc* LoadJSON(const char* path);
	JSON_Doc* CreateJSON(const char* path);


private:
	std::list<JSON_Doc*> jsons;

public:
	JSON_Doc* config = nullptr;
	JSON_Doc* default_config = nullptr;

	JSON_Doc* savescene = nullptr;
};

#endif // !__MODULE_PARSON_H__



