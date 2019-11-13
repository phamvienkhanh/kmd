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
    
}