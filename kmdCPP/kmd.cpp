// kmd.cpp : Defines the entry point for the console application.
//

#include "utilities.h"
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
	std::string 	m_gitBranch;

	static Kmd*		s_Instance;

	CONSOLE_SCREEN_BUFFER_INFO  info;

	bool 		m_isFistTimePressed;
	std::string m_currentCommand;
	int 		m_currIdxRcm;
	int 		m_currIdxHistory;

	std::vector<WIN32_FIND_DATA>  m_listFileRecommend;
	std::vector<std::string> 	  m_listHistoryCmd;	

private:
	Kmd();

	void Init();
	
	bool ShortDir(std::string _mdir, std::string &_rs);
	void SetColorAndBackground(char fr, char bg);

	void ResetColor();

	void PrintWorkingDir(std::string _path);

	void ExecuteCommand(const char * _cmd);
	void ClearCommand();

	void HandleTabKey();
	void HandleArrowKey(char ch);

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

	m_CurrentPath 	= GetCurrentWorkingDir();
	m_gitBranch 	= Utilities::GetGitBranch(m_CurrentPath);	
	PrintWorkingDir(m_CurrentPath);

	while (true)
	{
		//wait for get command
		char ch = _getch();
		//std::wcout << int(ch);
		//std::wcout << int( _getch());

		if(ch == 9) //tab key
		{
			HandleTabKey();
		} 
		else
		{
			m_isFistTimePressed = false;

			if(ch == 13) //enter key
			{
				std::wcout<< std::endl;
				
				ExecuteCommand(m_currentCommand.c_str());

				if(std::find(m_listHistoryCmd.begin(),m_listHistoryCmd.end(),m_currentCommand) == m_listHistoryCmd.end())
				{
					m_listHistoryCmd.push_back(m_currentCommand);
				}


				if(m_currentCommand != "cls")
					std::wcout<< std::endl;

				m_CurrentPath 	= GetCurrentWorkingDir();
				m_gitBranch 	= Utilities::GetGitBranch(m_CurrentPath);

				PrintWorkingDir(m_CurrentPath);
				m_currentCommand = "";
			}
			else if(ch == 8) //backspace key
			{
				if(m_currentCommand.size() > 0)
				{
					std::wcout<<"\b \b";
					m_currentCommand.pop_back();
				}
			}
			else if ( ch == -32 ) // up, down, right and left
			{
				HandleArrowKey(_getch());
			}
			else
			{
				m_isFistTimePressed = false;
				m_currentCommand += ch;
				std::wcout<< char(ch);
			}
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

	m_isFistTimePressed = false;
	m_currentCommand	= "";
	m_currIdxRcm 		= 0;
	m_currIdxHistory 	= 0;
	m_gitBranch 		= "";
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
	SetColorAndBackground(0, 5);
	std::wcout << L"KeithPham";

	//
	SetColorAndBackground(5, 1);
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

	
	if (m_gitBranch != "")
	{

		//set color for last symbol |>
		SetColorAndBackground(2, 3);
		std::wcout << L"\ue0b0 ";

		// print git branch name
		SetColorAndBackground(14, 3);
		std::wcout << L"\ue0a0 ";
		SetColorAndBackground(7, 3);
		std::wcout << m_gitBranch.c_str() << L" ";
		SetColorAndBackground(3, m_wOldAttributes >> 4);
		std::wcout << L"\ue0b0 ";
	}
	else
	{
		//set color for last symbol |>
		SetColorAndBackground(2, m_wOldAttributes >> 4);
		std::wcout << L"\ue0b0 ";
	}
	


	ResetColor();

	std::wcout << std::endl << "$ ";
}

void Kmd::ExecuteCommand(const char* _cmd)
{
	if (std::strlen(_cmd) != 0)
	{
		//if change directory
		if (Utilities::ParseCommand(_cmd) == CommandType::CHANGE_DIR)
		{
			std::string _chpath = "";
			for (int i = 3; i < std::strlen(_cmd); i++)
			{
				_chpath += _cmd[i];
			}

			_chdir(_chpath.c_str());
		}
		else if(Utilities::ParseCommand(_cmd) == CommandType::LIST_FILE)
		{
			std::vector<WIN32_FIND_DATA> listFiles;
			Utilities::GetListFiles(m_CurrentPath,listFiles);
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

void Kmd::ClearCommand()
{
	while(m_currentCommand.size() > 0)
	{
		std::wcout<<"\b \b";
		m_currentCommand.pop_back();
	}
}

void Kmd::HandleTabKey()
{
    if(!m_isFistTimePressed)
    {
        m_currIdxRcm           = 0;
        m_isFistTimePressed    = true;
        m_listFileRecommend.clear();

        std::string inputPath       = "";
        std::string macthFileName   = "";

        while(m_currentCommand.size() > 0 && (m_currentCommand.back() != ' '))
        {
            std::wcout<<"\b \b";
            inputPath += m_currentCommand.back();
            m_currentCommand.pop_back();
        }

        std::reverse(inputPath.begin(), inputPath.end());

		while(inputPath.size() > 0 && (inputPath.back() != '\\'))
		{
			macthFileName.push_back(inputPath.back());
			inputPath.pop_back();
		}

		if(inputPath.empty())
		{
			inputPath = ".\\" + inputPath;
		}
		
		std::reverse(macthFileName.begin(), macthFileName.end());
		std::transform(macthFileName.begin(), macthFileName.end(), macthFileName.begin(), [](unsigned char c){ return std::tolower(c); });
		Utilities::GetListFiles(inputPath,m_listFileRecommend);

		std::vector<WIN32_FIND_DATA> filterFile;
		for(auto& file : m_listFileRecommend)
		{
			std::string strFilename = file.cFileName;
			std::transform(strFilename.begin(), strFilename.end(), strFilename.begin(), [](unsigned char c){ return std::tolower(c); });
			if(strFilename.find(macthFileName) != std::string::npos)
			{
				filterFile.push_back(file);

			}
		}

		m_listFileRecommend = filterFile;

		m_currentCommand = m_currentCommand + inputPath + std::string(m_listFileRecommend[m_currIdxRcm].cFileName);
		std::wcout<< inputPath.c_str() <<m_listFileRecommend[m_currIdxRcm].cFileName;
    }
    else
    {
        if(++m_currIdxRcm >= m_listFileRecommend.size())
            m_currIdxRcm = 0;

        while(m_currentCommand.size() > 0 && (m_currentCommand.back() != '\\'))
        {
            std::wcout<<"\b \b";
            m_currentCommand.pop_back();
        }
        m_currentCommand += std::string(m_listFileRecommend[m_currIdxRcm].cFileName);
        std::wcout<< m_listFileRecommend[m_currIdxRcm].cFileName;
    }
}


void Kmd::HandleArrowKey(char ch)
{
	if(m_listHistoryCmd.size() > 0)
	{
		if( ch == 72 ) // up 
		{
			m_currIdxHistory++;
		}
		else if( ch == 80 ) // down 
		{
			m_currIdxHistory--;
		}
		else if ( ch == 77 ) // right
		{

		}
		else // ch == 75 
		{
			std::wcout << "\b";
		}

		if(m_currIdxHistory >= (int)m_listHistoryCmd.size())
		{
			m_currIdxHistory = 0;
		}
		else if(m_currIdxHistory < 0)
		{
			m_currIdxHistory = m_listHistoryCmd.size() - 1;
		}

		ClearCommand();
		m_currentCommand = m_listHistoryCmd[m_currIdxHistory];
		std::wcout << m_currentCommand.c_str();
		
		
	}
}

int main()
{
	Kmd::GetInstance()->Run();
	return 0;
}
