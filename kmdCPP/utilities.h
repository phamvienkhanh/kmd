#include "stdafx.h"

#ifndef GetCurrentDir
#define GetCurrentDir _getcwd
#endif

#ifndef _O_U16TEXT
#define _O_U16TEXT 0x20000
#endif

enum CommandType 
{
	CHANGE_DIR = 0,
	LIST_FILE,
	
	COUNT
};

namespace Utilities 
{
    CommandType ParseCommand(const char* _cmd);
    void GetListFiles(const std::string& name, std::vector<WIN32_FIND_DATA>& v);
}