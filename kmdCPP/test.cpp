#include "stdafx.h"

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

int main( void )
{
   	int ch;
	std::string strcmd="";
   	do
   	{
      		ch = _getch();
		if(ch == 13)
		{
			std::cout <<std::endl<< strcmd << std::endl;
			strcmd = "";
			std::vector<WIN32_FIND_DATA> v;
			GetDirectTree("D:\\Games\\do", v);
			for(auto& i : v)
			{
				std::cout<< i.cFileName <<std::endl;
			}
		}
		else if(ch == 8)
		{
			if(strcmd.size() > 0)
			{
				std::cout<<"\b \b";
				strcmd.pop_back();
			}
		}
		else if(ch == 9)
		{
			while(strcmd.size() > 0 && (strcmd.back() != ' '))
			{
				std::cout<<"\b \b";
				strcmd.pop_back();
			}
			strcmd += "recommend";
			std::cout << "recommend";
		}
		else
		{
			strcmd += ch;
			std::cout<< char(ch);
		}
   	} while(1);
 	
}