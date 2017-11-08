#pragma once

#ifndef __HELPER_FOOS_H__
#define __HELPER_FOOS_H__

#include <string>

char* Capitalize(char* str);

std::string GetFileFromPath(const char* path, bool take_extension = true);
const char* GetExtension(const char* path);

#endif // !__HELPER_FOOS_H__
