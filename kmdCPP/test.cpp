#include "stdafx.h"
#include "utilities.h"

using namespace std;
int currTagetMenu = 0;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // used for goto
COORD CursorPosition; // used for goto

void gotoXY(int,int); // function defined below if this is new to you.

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        COORD invalid = { 0, 0 };
        return invalid;
    }
}


void gotoXY(int x, int y) 
{ 
	CursorPosition.X = x; 
	CursorPosition.Y = y; 
	SetConsoleCursorPosition(hConsole,CursorPosition); 
}

void ClearScreen(int top, int left, int bottom, int right)
{
	std::string spaceChar(right - left,' ');
	for(int i = top+1; i <= bottom; i++ )
	{
		gotoXY(i,left);
		std::cout << spaceChar;
	}

	gotoXY(top,left);
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
			GetDirectTree(".\\", v);
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
			/*while(strcmd.size() > 0 && (strcmd.back() != ' '))
			{
				std::cout<<"\b \b";
				strcmd.pop_back();
			}*/
			COORD savePos = GetConsoleCursorPosition(hConsole);
			std::vector<WIN32_FIND_DATA> v;
			GetDirectTree(".\\", v);
			gotoXY(savePos.X, savePos.Y+1);
			for(int i=0; i < v.size(); i++)
			{
				std::string _temp = currTagetMenu == i ? "-->" : "   ";
				std::cout<< _temp <<v[i].cFileName <<std::endl;
			}
			gotoXY(savePos.X, savePos.Y+1);
		}
		else if(ch == 224)
		{
			char c2 = _getch();
			COORD savePos = GetConsoleCursorPosition(hConsole);
			//savePos.X -= 3;

			//gotoXY(savePos.X, savePos.Y);
			std::cout << "   ";

			if (c2 == 75) savePos.Y++;
			else savePos.Y--;

			gotoXY(0, savePos.Y);
			std::cout << "-->";
			gotoXY(0, savePos.Y);

		}
		else
		{
			strcmd += ch;
			std::cout<< ch;
		}
   	} while(1);
 	
}