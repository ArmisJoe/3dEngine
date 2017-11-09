#include "ModuleParson.h"
#include "Application.h"
#include <string>

ModuleParson::ModuleParson(Application * app, bool start_enabled) : Module(app, start_enabled)
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

JSON_Doc * ModuleParson::LoadJSON(const char * path)
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
		CreateJSON(path);
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
