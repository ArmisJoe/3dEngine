#pragma once

#ifndef __HELPER_FOOS_H__
#define __HELPER_FOOS_H__

#include <string>

char* Capitalize(char* str) {
	for (int i = 0; str[i] != '\0'; i++)
	{
		str[i] = toupper(str[i]);
	}
	return str;
}

string GetFileFromPath(const char* path) {
	string ret;
	
	for (int i = 0; path[i] != '\0'; i++) {
		if (path[i] == '\\' || path[i] == '/') {
			ret.clear();
			continue;
		}
		ret += path[i];
	}

	return ret;
}

#endif // !__HELPER_FOOS_H__
