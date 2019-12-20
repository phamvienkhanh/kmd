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

enum EColor
{
	Black = 0,
	Blue,
	Green,
	Aqua,
	Red,
	Purple,
	Yellow,
	White,
	Gray,
	LightBlue,
	LightGreen,
	LightAqua,
	LightRed,
	LightPurple,
	LightYellow,
	BrightWhite,

	COUNT_COLOR

};

namespace Utilities 
{
    CommandType ParseCommand(const char* _cmd);
    void GetListFiles(const std::string& name, std::vector<WIN32_FIND_DATA>& v);

	bool DirectoryExists(LPCTSTR szPath);


	std::string GetGitBranch(std::string currentPath);
    std::string exec(const char* cmd);

    COORD GetConsoleCursorPosition(HANDLE hConsoleOutput);
}