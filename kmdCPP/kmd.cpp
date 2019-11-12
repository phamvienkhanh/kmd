// kmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define GetCurrentDir _getcwd

#ifndef _O_U16TEXT
#define _O_U16TEXT 0x20000
#endif


enum CommandType 
{
	CHANGE_DIR = 0,
	LIST_FILE,
	
	COUNT
};

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
    if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
        do {
            v.push_back(data);
        } while (FindNextFile(hFind, &data) != 0);
        FindClose(hFind);
    }
}

//singleton class
class Kmd
{
public:
	static Kmd* GetInstance();

	void Run();

	std::string GetCurrentWorkingDir();

private:
	HANDLE			hConsole;

	WORD			m_wOldAttributes;
	std::string		m_CurrentPath;
	static Kmd*		s_Instance;

	CONSOLE_SCREEN_BUFFER_INFO  info;

private:
	Kmd();

	void Init();
	
	bool ShortDir(std::string _mdir, std::string &_rs);
	void SetColorAndBackground(char fr, char bg);

	void ResetColor();

	void PrintWorkingDir(std::string _path);

	void ExecuteCommand(const char * _cmd);

};

Kmd* Kmd::s_Instance = nullptr;

Kmd * Kmd::GetInstance()
{
	if (s_Instance == nullptr)
	{
		s_Instance = new Kmd();
	}

	return s_Instance;
}

void Kmd::Run()
{
	std::string strcmd="";
	
	m_CurrentPath = GetCurrentWorkingDir();	
	PrintWorkingDir(m_CurrentPath);

	while (true)
	{
		// char cmd[256];
		// std::cin.getline(cmd, 256);

		//wait for get command
		char ch = _getch();

		if(ch == 13) //enter key
		{
			std::wcout<< std::endl;
			ExecuteCommand(strcmd.c_str());

			if(strcmd != "cls")
				std::wcout<< std::endl;

			m_CurrentPath = GetCurrentWorkingDir();	
			PrintWorkingDir(m_CurrentPath);
			strcmd = "";
		}
		else if(ch == 8) //backspace key
		{
			if(strcmd.size() > 0)
			{
				std::wcout<<"\b \b";
				strcmd.pop_back();
			}
		}
		else if(ch == 9) //tab key
		{
			while(strcmd.size() > 0 && (strcmd.back() != ' '))
			{
				std::wcout<<"\b \b";
				strcmd.pop_back();
			}
			strcmd += "recommend";
			std::wcout << "recommend";
		}
		else
		{
			strcmd += ch;
			std::wcout<< char(ch);
		}
	}
}

Kmd::Kmd()
{
	Init();
}

void Kmd::Init()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//get and save current attributes of console
	if (!GetConsoleScreenBufferInfo(hConsole, &info))
		return;
	m_wOldAttributes = info.wAttributes;

	SetConsoleTitle("KityKeith-Kmd");

	//set code page utf-16
	_setmode(_fileno(stdout), _O_U16TEXT);
}

std::string Kmd::GetCurrentWorkingDir()
{
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

bool Kmd::ShortDir(std::string _mdir, std::string &_rs)
{
	_mdir.erase(_mdir.begin(), _mdir.begin() + 2);
	std::string _strtmp = "";

	//if dir string too long, then get 4 level of dir
	if (_mdir.length() > 20)
	{
		int _countSlash = 0;
		for (int i = _mdir.length() - 1; i > 0; i--)
		{
			if (_mdir[i] == '\\')
			{
				if (++_countSlash == 3)
				{
					_rs = _strtmp;
					return true;
				}
			}

			_strtmp = _mdir[i] + _strtmp;
		}
	}

	_rs = _mdir;

	return false;
}

void Kmd::SetColorAndBackground(char fr, char bg)
{
	// colorattribute = foreground + background * 16
	//0 = Black 8 = Gray
	//1 = Blue 9 = Light Blue
	//2 = Green a = Light Green
	//3 = Aqua b = Light Aqua
	//4 = Red c = Light Red
	//5 = Purple d = Light Purple
	//6 = Yellow e = Light Yellow
	//7 = White f = Bright White

	unsigned char _color = 0;
	_color = (bg << 4) | fr;
	SetConsoleTextAttribute(hConsole, _color);
}

void Kmd::ResetColor()
{
	//reset color into oldColor 
	SetConsoleTextAttribute(hConsole, m_wOldAttributes);
}

void Kmd::PrintWorkingDir(std::string _path)
{
	// print symbol 
	SetColorAndBackground(4, 8);
	std::wcout << L"KeithPham";

	//
	SetColorAndBackground(8, 1);
	std::wcout << L"\ue0b0 ";

	//set color for partition
	SetColorAndBackground(14, 1); //white - blue
	std::wcout << _path[0] << L" ";

	//set color for symbol |>
	SetColorAndBackground(1, 2); //blue - green
	std::wcout << L"\ue0b0 ";

	//set color for dir
	SetColorAndBackground(0, 2); //black - green
	std::string _dir;
	if (ShortDir(_path, _dir))
	{
		std::wcout << L"\u21b3\\" << _dir.c_str();
	}	
	else
	{
		std::wcout << _dir.c_str();
	}

	//set color for last symbol |>
	SetColorAndBackground(2, m_wOldAttributes >> 4);
	std::wcout << L"\ue0b0 ";

	ResetColor();

	std::wcout << std::endl << "$ ";
}

void Kmd::ExecuteCommand(const char* _cmd)
{
	if (std::strlen(_cmd) != 0)
	{
		//if change directory
		if (ParseCommand(_cmd) == CommandType::CHANGE_DIR)
		{
			std::string _chpath = "";
			for (int i = 3; i < std::strlen(_cmd); i++)
			{
				_chpath += _cmd[i];
			}

			_chdir(_chpath.c_str());
		}
		else if(ParseCommand(_cmd) == CommandType::LIST_FILE)
		{
			std::vector<WIN32_FIND_DATA> listFiles;
			std::string currentDir = GetCurrentWorkingDir();
			GetDirectTree(currentDir,listFiles);
			for(auto& i : listFiles)
			{
				std::wcout<< i.cFileName <<std::endl;
			}
		}
		else
		{
			//execute cmd
			system(_cmd);
		}
	}
}

int main()
{
	Kmd::GetInstance()->Run();
	return 0;
}
