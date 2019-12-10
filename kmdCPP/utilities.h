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

	inline std::string exec(const char* cmd) 
    {
        char buffer[128];
        std::string result = "";
        FILE* pipe = _popen(cmd, "r");
        if (!pipe) throw std::runtime_error("popen() failed!");
        try {
            while (fgets(buffer, sizeof buffer, pipe) != NULL) {
                result += buffer;
            }
        } catch (...) {
            _pclose(pipe);
            throw;
        }
        _pclose(pipe);
        return result;
    }


	inline std::string GetGitBranch()
    {
        std::string branchname = "";
        branchname = exec("git rev-parse --abbrev-ref HEAD");

        if(branchname.find("fatal:") != std::string::npos)
        {
            branchname = "";
        }

        return branchname;
    }
}