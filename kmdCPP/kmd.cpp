// kmd.cpp : Defines the entry point for the console application.

#include "kmd.h"


Kmd* Kmd::s_Instance = nullptr;

Kmd* Kmd::GetInstance()
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

//std::wcout<<"Kmd::KeyEventProc \n";

	DWORD cNumRead, fdwMode, fdwSaveOldMode; 
    INPUT_RECORD irInBuf[128]; 
//std::wcout<<"Kmd::KeyEventProc \n";

    if (! GetConsoleMode(hConsoleIn, &fdwSaveOldMode) ) 
        ErrorExit("GetConsoleMode"); 

//std::wcout<<"Kmd::KeyEventProc \n";
    // Enable the window and mouse input events. 
 
    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT; 
    if (! SetConsoleMode(hConsoleIn, fdwMode) ) 
        ErrorExit("SetConsoleMode");
//std::wcout<<"Kmd::KeyEventProc \n";
	while (true)
	{
//std::wcout<<"Kmd::KeyEventProc \n";
		//wait for get command
		if (! ReadConsoleInput(hConsoleIn, irInBuf, 128, &cNumRead)) 
            ErrorExit("ReadConsoleInput");
//std::wcout<<"Kmd::KeyEventProc \n";
		// Dispatch the events to the appropriate handler. 
        for (WORD i = 0; i < cNumRead; i++) 
        {
            switch(irInBuf[i].EventType) 
            { 
                case KEY_EVENT: // keyboard input 
                    KeyEventProc(irInBuf[i].Event.KeyEvent); 
                    break; 
 
                case MOUSE_EVENT: // mouse input 
                    MouseEventProc(irInBuf[i].Event.MouseEvent); 
                    break; 
 
                case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
                    ResizeEventProc( irInBuf[i].Event.WindowBufferSizeEvent ); 
                    break; 
 
                case FOCUS_EVENT:  // disregard focus events 
 
                case MENU_EVENT:   // disregard menu events 
                    break; 
 
                default:
                	std::wcout<<"Kmd::KeyEventProc \n";
                    ErrorExit("Unknown event type"); 
                    break; 
            } 
        }
	}

	SetConsoleMode(hConsoleIn, fdwSaveOldMode);
}

Kmd::Kmd()
{
	Init();
}

void Kmd::Init()
{
	hConsoleOut 		= GetStdHandle(STD_OUTPUT_HANDLE);
	hConsoleIn 			= GetStdHandle(STD_INPUT_HANDLE); 
	m_isFistTimePressed = false;
	m_currentCommand	= "";
	m_currIdxRcm 		= 0;
	m_currIdxHistory 	= 0;
	m_currIdxCommand 	= 0;
	m_gitBranch 		= "";
	m_endLinePos 		= {0,0};
	m_currCursorPos     = {0,0};

	m_menuPopup = new MenuPopup(hConsoleOut);
	m_menuPopup->SetCBEventEnter([](std::string item){
		Kmd::GetInstance()->CBMenuEventEnter(item);
	});

	//get and save current attributes color of console
	CONSOLE_SCREEN_BUFFER_INFO  info;
	if (!GetConsoleScreenBufferInfo(hConsoleOut, &info))
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
	std::string title = " --o0Kmd0o--   " + current_working_dir ;
	SetConsoleTitle(title.c_str());
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
	//1 = Blue 9 = LightBlue
	//2 = Green a = LightGreen
	//3 = Aqua b = LightAqua
	//4 = Red c = LightRed
	//5 = Purple d = LightPurple
	//6 = Yellow e = LightYellow
	//7 = White f = BrightWhite

	unsigned char _color = 0;
	_color = (bg << 4) | fr;
	SetConsoleTextAttribute(hConsoleOut, _color);
}

void Kmd::ResetColor()
{
	//reset color into oldColor 
	SetConsoleTextAttribute(hConsoleOut, m_wOldAttributes);
}

void Kmd::PrintWorkingDir(std::string _path)
{
	// print symbol 
	SetColorAndBackground(EColor::Black, EColor::Purple);
	std::wcout << L"KeithPham";

	//
	SetColorAndBackground(EColor::Purple, EColor::Blue);
	std::wcout << L"\ue0b0 ";

	//set color for partition
	SetColorAndBackground(EColor::LightYellow, EColor::Blue); //white - blue
	std::wcout << _path[0] << L" ";

	//set color for symbol |>
	SetColorAndBackground(EColor::Blue, EColor::Green); //blue - green
	std::wcout << L"\ue0b0 ";

	//set color for dir
	SetColorAndBackground(EColor::Black, EColor::Green); //black - green
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
		SetColorAndBackground(EColor::Green, EColor::Aqua);
		std::wcout << L"\ue0b0 ";

		// print git branch name
		SetColorAndBackground(EColor::LightYellow, EColor::Aqua);
		std::wcout << L"\ue0a0 ";
		SetColorAndBackground(EColor::White, EColor::Aqua);
		std::wcout << m_gitBranch.c_str() << L" ";
		SetColorAndBackground(EColor::Aqua, m_wOldAttributes >> 4);
		std::wcout << L"\ue0b0 ";
	}
	else
	{
		//set color for last symbol |>
		SetColorAndBackground(EColor::Green, m_wOldAttributes >> 4);
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
				if(i.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
					SetColorAndBackground(EColor::Green, m_wOldAttributes >> 4);
					std::wcout<< i.cFileName <<std::endl;
					ResetColor();
				}
				else
				{
					std::wcout<< i.cFileName << std::endl;
				}

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

int main()
{
	Kmd::GetInstance()->Run();
	return 0;
}
