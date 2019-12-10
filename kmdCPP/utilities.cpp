#include "utilities.h"

namespace Utilities 
{
    /*
        @param 
            _cmd : string command
        @return
             enum CommandType : type of command 
    */
    CommandType ParseCommand(const char* _cmd)
    {
        if((std::strlen(_cmd) > 3) && (_cmd[0] == 'c') && (_cmd[1] == 'd') && (_cmd[2] == ' '))
        {
            return CommandType::CHANGE_DIR;
        }
        else if((std::strlen(_cmd) == 2) && (_cmd[0] == 'l') && (_cmd[1] == 's'))
        {
            return CommandType::LIST_FILE;
        }
        
        return CommandType::COUNT;
    }

    /*
        @input 
            path : path to directory
        @output
            list of files in path
    */
    void GetListFiles(const std::string& path, std::vector<WIN32_FIND_DATA>& listFiles)
    {
        std::string pattern(path);
        pattern.append("\\*");
        WIN32_FIND_DATA data;
        HANDLE hFind;
        if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) 
        {
            do {
                listFiles.push_back(data);
            } while (FindNextFile(hFind, &data) != 0);
            FindClose(hFind);
        }
    }

    bool DirectoryExists(LPCTSTR szPath)
    {
      DWORD dwAttrib = GetFileAttributes(szPath);

      return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
             (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    std::string exec(const char* cmd) 
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

    std::string GetGitBranch(std::string currentPath)
    {
        int levelDir = std::count(currentPath.begin(), currentPath.end(), '\\');
        std::string gitPath = ".git";

        bool found = false;

        for(int i = 1; i < levelDir ; i++)
        {
            if(DirectoryExists(gitPath.c_str()))
            {
                found = true;
                break;
            }

            gitPath = "..\\" + gitPath;
        }

        std::string gitbranchName = "";
        if(found)
        {
            gitbranchName = exec("git rev-parse --abbrev-ref HEAD");
            gitbranchName = std::string(gitbranchName.begin(), gitbranchName.end() - 1);
        }

        return gitbranchName;
    }
}