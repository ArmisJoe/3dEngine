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

#endif // !__HELPER_FOOS_H__
