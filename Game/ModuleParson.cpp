#include "ModuleParson.h"
#include "Application.h"
#include <string>


ModuleParson::ModuleParson(bool start_enabled) : Module(start_enabled)
{
	name = "Parson";
}

ModuleParson::~ModuleParson()
{
}

bool ModuleParson::Init()
{
	LOG("Module Parson Init");
	config = LoadJSON("Settings/config.json");
	default_config = LoadJSON("Settings/default_config.json");
	return true;
}

bool ModuleParson::CleanUp()
{
	bool ret = true;

	LOG("Unloading JSON Module");

	for (list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end();)
	{
		(*it)->CleanUp();
		mdelete (*it);

		it = jsons.erase(it);
	}

	return ret;
}

JSON_Doc * ModuleParson::LoadJSON(const char * path, bool create_new)
{
	JSON_Doc* ret = nullptr;

	//bool exists = false;
	//for (list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end(); it++)
	//{
	//	string doc_str = (*it)->GetPath();
	//	if (doc_str.compare(path))
	//	{
	//		ret = (*it);
	//		exists = true;
	//		break;
	//	}
	//}

	if (!App->fs->exists(path)) {
		if(create_new)
			CreateJSON(path);
		LOG("File Does not Exist");
	}
	if(App->fs->exists(path)){
		JSON_Value *user_data = json_parse_file(path);
		JSON_Object *root_object = json_value_get_object(user_data);

		if (user_data == nullptr)
		{
			LOG("Error loading %s", path);
		}
		else
		{
			LOG("Succes loading %s", path);

			JSON_Doc* new_doc = new JSON_Doc(user_data, root_object, path);
			jsons.push_back(new_doc);

			ret = new_doc;
			ret->Save();
		}
	} 
	return ret;
}

JSON_Doc * ModuleParson::CreateJSON(const char * path)
{
	JSON_Doc* ret = nullptr;

	//bool exists = false;
	//for (list<JSON_Doc*>::iterator it = jsons.begin(); it != jsons.end(); it++)
	//{
	//	string doc_str = (*it)->GetPath();
	//	if (doc_str.compare(path))
	//	{
	//		exists = true;
	//		break;
	//	}
	//}

	if (App->fs->exists(path))
	{
		LOG("Error creating %s. There is already a file with this path/name", path);
	}
	else
	{
		JSON_Value* root_value = json_value_init_object();

		if (root_value == nullptr)
		{
			LOG("Error creating %s. Wrong path?", path);
		}
		else
		{

			JSON_Object* root_object = json_value_get_object(root_value);

			JSON_Doc* new_doc = new JSON_Doc(root_value, root_object, path);
			jsons.push_back(new_doc);

			ret = new_doc;
			ret->Save();
		}
	}

	return ret;
}

JSON_Doc::JSON_Doc(JSON_Value * value, JSON_Object * object, const char * path)
{
	this->value = value;
	this->object = object;
	this->root = object;
	this->path = path;
}

JSON_Doc::~JSON_Doc()
{
}

void JSON_Doc::SetString(const char * set, const char * str)
{
	json_object_dotset_string(object, set, str);
}

void JSON_Doc::SetBool(const char * set, bool bo)
{
	json_object_dotset_boolean(object, set, bo);
}

void JSON_Doc::SetNumber(const char * set, double nu)
{
	json_object_dotset_number(object, set, nu);
}

void JSON_Doc::SetArray(const char * set)
{
	JSON_Value* va = json_value_init_array();
	JSON_Array* jarr = json_value_get_array(va);

	json_object_dotset_value(object, set, va);

}

const int JSON_Doc::GetArraySize(const char * arr)
{
	int ret = 0;

	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr)
		ret = json_array_get_count(jarr);

	return ret;
}

const char * JSON_Doc::GetStringArr(const char * arr, int idx)
{
	const char* ret = nullptr;
	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr)
		if (FindArrayValue(arr, idx, json_value_type::JSONString))
			ret = json_array_get_string(jarr, idx);
	return ret;
}

const bool JSON_Doc::GetBoolArr(const char * arr, int idx)
{
	bool ret = false;
	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr)
		if (FindArrayValue(arr, idx, json_value_type::JSONBoolean))
			ret = json_array_get_boolean(jarr, idx);
	return ret;
}

const double JSON_Doc::GetNumberArr(const char * arr, int idx)
{
	double ret = 0;
	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr)
		if (FindArrayValue(arr, idx, json_value_type::JSONNumber))
			ret = json_array_get_number(jarr, idx);
	return ret;
}

void JSON_Doc::AddStringArr(const char * arr, const char * str)
{
	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr)
		json_array_append_string(jarr, str);
}

void JSON_Doc::AddBoolArr(const char * arr, bool bo)
{
	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr)
		json_array_append_boolean(jarr, bo);
}

void JSON_Doc::AddNumberArr(const char * arr, double nu)
{
	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr)
		json_array_append_number(jarr, nu);
}

void JSON_Doc::AddSectionArr(const char* arr) {
	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr)
		json_array_append_value(jarr, json_value_init_object());
}

bool JSON_Doc::FindArrayValue(const char * arr, int idx, json_value_type type)
{
	bool ret = false;

	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr) {
		JSON_Value* value = json_array_get_value(jarr, idx);
		if (value != nullptr && json_value_get_type(value) == type)
			ret = true;
	}


	return ret;
}

bool JSON_Doc::MoveToSectionInsideArr(const char * arr, int idx)
{
	bool ret = false;

	JSON_Array* jarr = json_object_get_array(object, arr);
	if (jarr != nullptr) {
		JSON_Object* jobj = json_array_get_object(jarr, idx);
		object = jobj;

		ret = true;
	}

	return ret;
}

const char * JSON_Doc::GetString(const char * str)
{
	return json_object_dotget_string(object, str);
}

bool JSON_Doc::GetBool(const char * bo)
{
	return json_object_dotget_boolean(object, bo);
}

double JSON_Doc::GetNumber(const char * nu)
{
	return json_object_dotget_number(object, nu);
}

const char * JSON_Doc::GetPath()
{
	return path;
}

void JSON_Doc::Save()
{
	json_serialize_to_file_pretty(value, path);
}

void JSON_Doc::CleanUp()
{
	json_value_free(value);
}

void JSON_Doc::MoveToRootSection() {
	object = root;
}

bool JSON_Doc::MoveToSection(const char* section) {
	bool ret = false;

	JSON_Object* jobj = json_object_get_object(object, section);

	if (jobj != nullptr) {
		object = jobj;
		ret = true;
	}

	return ret;
}