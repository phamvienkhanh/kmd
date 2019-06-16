// kmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstring>
#include "ostream_wide.h"
#include "wostream_wide.h"

#define GetCurrentDir _getcwd


//singleton class
class Kmd
{
public:
	static Kmd* GetInstance();

	void Run();

private:
	HANDLE			hConsole;
	WORD			m_wOldAttributes;
	std::string		m_CurrentPath;
	static Kmd*		s_Instance;

	CONSOLE_SCREEN_BUFFER_INFO  info;

private:
	Kmd();

	void Init();
	std::string GetCurrentWorkingDir();
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
	while (true)
	{
		m_CurrentPath = GetCurrentWorkingDir();

		PrintWorkingDir(m_CurrentPath);

		//wait for get command
		char cmd[256];
		std::cin.getline(cmd, 256);

		ExecuteCommand(cmd);

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

	//set code page utf-8
	//SetConsoleOutputCP(65001);
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
	std::cout << L" ⚡ ";

	//
	SetColorAndBackground(8, 1);
	std::cout << L" ";

	//set color for partition
	SetColorAndBackground(14, 1); //white - blue
	std::cout << _path[0] << " ";

	//set color for symbol |>
	SetColorAndBackground(1, 2); //blue - green
	std::cout << L"";

	//set color for dir
	SetColorAndBackground(0, 2); //black - green
	std::string _dir;
	if (ShortDir(_path, _dir))
	{
		std::cout << L" ↪ " << _dir.c_str();
	}	
	else
	{
		std::cout << _dir.c_str();
	}

	//set color for last symbol |>
	SetColorAndBackground(2, m_wOldAttributes >> 4);
	std::cout << L"";

	ResetColor();
}

void Kmd::ExecuteCommand(const char* _cmd)
{
	if (std::strlen(_cmd) != 0)
	{
		//if change directory
		if ((std::strlen(_cmd) > 3) && (_cmd[0] == 'c') && (_cmd[1] == 'd') && (_cmd[2] == ' '))
		{
			std::string _chpath = "";
			for (int i = 3; i < std::strlen(_cmd); i++)
			{
				_chpath += _cmd[i];
			}

			_chdir(_chpath.c_str());
		}
		else if ((std::strlen(_cmd) > 1) && (_cmd[0] == 'l') && (_cmd[1] == 's'))
		{
			char _buffer[128];
			//std::string _result = "";
			FILE* pipe = _popen(_cmd, "r");
			//int _countColum = 0;
			//get result of cmd
			while (fgets(_buffer, sizeof(_buffer), pipe) != NULL)
			{
				//_result += _buffer;
				char _pathName[128];
				strncpy_s(_pathName, _buffer, strlen(_buffer) - 1);
				struct stat _info;
				stat(_pathName, &_info);
				if (_info.st_mode & _S_IFDIR)
				{
					SetColorAndBackground(1, m_wOldAttributes >> 4);
					printf_s("%s", _buffer);
					ResetColor();
				}
				else
				{
					printf_s("%s", _buffer);
				}

				/*if (++_countColum < 4)
				{
				//printf_s("\t");
				}
				else
				{
				printf_s("\n");
				_countColum = 0;
				}*/

			}

			//printf_s("\n");

			_pclose(pipe);
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
