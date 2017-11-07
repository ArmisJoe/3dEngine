#include "HelperFoos.h"

char * Capitalize(char * str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		str[i] = toupper(str[i]);
	}
	return str;
}

std::string GetFileFromPath(const char * path)
{
	std::string ret;

	for (int i = 0; path[i] != '\0'; i++) {
		if (path[i] == '\\' || path[i] == '/') {
			ret.clear();
			continue;
		}
		ret += path[i];
	}

	return ret;
}
