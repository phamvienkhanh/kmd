#include "kmd.h"

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


void Kmd::HandleArrowKey(WORD ch)
{
	if (ch == VK_RIGHT) // right
	{
		COORD pos = Utilities::GetConsoleCursorPosition(hConsoleOut);
		if(pos.X++ < m_endLinePos.X)
			SetConsoleCursorPosition(hConsoleOut,pos);
	}
	else if (ch == VK_LEFT) 
	{
		COORD pos = Utilities::GetConsoleCursorPosition(hConsoleOut);
		if(pos.X-- > 2)
			SetConsoleCursorPosition(hConsoleOut,pos);
	}
	else
	{
		if(m_listHistoryCmd.size() > 0)
		{
			if(ch == VK_UP) // up 
			{
				m_currIdxHistory++;
			}
			else //if( ch == 80 ) // down 
			{
				m_currIdxHistory--;
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
}

void Kmd::UpdateCursorPos(bool shouldUpdateEndLinePos)
{
	m_currCursorPos   = Utilities::GetConsoleCursorPosition(hConsoleOut);
	m_currIdxCommand  = m_currCursorPos.X - 2;

	if(shouldUpdateEndLinePos)
		m_endLinePos = m_currCursorPos;
}

void Kmd::KeyEventProc(KEY_EVENT_RECORD ker)
{
	//std::wcout<<"Kmd::KeyEventProc \n";

    if(ker.bKeyDown)
    {
        /*printf("bKeyDown            : %d\n" , ker.bKeyDown);
    	printf("wRepeatCount        : %d\n" , ker.wRepeatCount);
    	printf("wVirtualKeyCode     : %d\n" , ker.wVirtualKeyCode);
    	printf("wVirtualScanCode    : %d\n" , ker.wVirtualScanCode);
    	printf("AsciiChar           : %c\n" , ker.uChar.AsciiChar);
    	printf("UnicodeChar         : %c\n" , ker.uChar.UnicodeChar);
    	printf("dwControlKeyState   : %d\n" , ker.dwControlKeyState);*/
    

	    if(ker.wVirtualKeyCode == VK_TAB) //tab key
		{
			HandleTabKey();
		} 
		else
		{
			m_isFistTimePressed = false;

			if(ker.wVirtualKeyCode == VK_RETURN) //enter key
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
				m_currIdxCommand = 0;

				UpdateCursorPos(true);
			}
			else if(ker.wVirtualKeyCode == VK_BACK) //backspace key
			{
				if(m_currentCommand.size() > 0)
				{
					std::wcout<<"\b \b";
					m_currentCommand.pop_back();
					UpdateCursorPos(true);
				}
			}
			else if (ker.wVirtualKeyCode == VK_RIGHT 
						|| ker.wVirtualKeyCode == VK_DOWN
						|| ker.wVirtualKeyCode == VK_LEFT
						|| ker.wVirtualKeyCode == VK_UP) // up, down, right and left
			{
				HandleArrowKey(ker.wVirtualKeyCode);
				UpdateCursorPos(false);
			}
			else
			{
				m_isFistTimePressed = false;

				if(ker.uChar.AsciiChar != '\0')
				{
					if(m_endLinePos.X == m_currCursorPos.X)
					{
						m_currentCommand += ker.uChar.AsciiChar;
						std::wcout<< ker.uChar.AsciiChar;
						UpdateCursorPos(true);
					}
					else
					{
						m_currentCommand.insert(m_currIdxCommand,1,ker.uChar.AsciiChar);
						std::wcout<< m_currentCommand.substr(m_currIdxCommand).c_str();
						m_currCursorPos.X++;
						SetConsoleCursorPosition(hConsoleOut,m_currCursorPos);
						UpdateCursorPos(false);
					}
				}
			}
		}
	}
}

void Kmd::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
    printf("Mouse event: ");
    
    switch(mer.dwEventFlags)
    {
        case 0:

            if(mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            {
                printf("left button press \n");
            }
            else if(mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
            {
                printf("right button press \n");
            }
            else
            {
                printf("button press\n");
            }
            break;
        case DOUBLE_CLICK:
            printf("double click\n");
            break;
        case MOUSE_HWHEELED:
            printf("horizontal mouse wheel\n");
            break;
        case MOUSE_MOVED:
            printf("mouse moved\n");
            break;
        case MOUSE_WHEELED:
            printf("vertical mouse wheel\n");
            break;
        default:
            printf("unknown\n");
            break;
    }
}

void Kmd::ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	printf("Resize event\n");
    printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}

void Kmd::ErrorExit (LPSTR lpszMessage) 
{ 
    /*fprintf(stderr, "%s\n", lpszMessage); 

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    ExitProcess(0); */
}