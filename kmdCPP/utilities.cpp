#include "utilities.h"

namespace Utilities 
{
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

    void GetDirectTree(const std::string& name, std::vector<WIN32_FIND_DATA>& v)
    {
        std::string pattern(name);
        pattern.append("\\*");
        WIN32_FIND_DATA data;
        HANDLE hFind;
        if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) 
        {
            do {
                v.push_back(data);
            } while (FindNextFile(hFind, &data) != 0);
            FindClose(hFind);
        }
    }
}