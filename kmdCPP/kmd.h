#include "utilities.h"
//singleton class

#ifndef __KMD_HEADER_H__
#define __KMD_HEADER_H__

class Kmd
{
public:
	static Kmd* GetInstance();

	void Run();

	std::string GetCurrentWorkingDir();

private:
	HANDLE			hConsoleOut;
	HANDLE			hConsoleIn;

	static Kmd*		s_Instance;

	WORD			m_wOldAttributes;
	std::string		m_CurrentPath;
	std::string 	m_gitBranch;

	COORD 			m_endLinePos;
	COORD           m_currCursorPos;

	bool 			m_isFistTimePressed;
	int 			m_currIdxRcm;
	int 			m_currIdxHistory;
	int    			m_currIdxCommand; // position of cursor on command
	std::string 	m_currentCommand;

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
	void HandleArrowKey(WORD ch);

	void UpdateCursorPos(bool shouldUpdateEndLinePos);

	void ErrorExit (LPSTR lpszMessage);
	void KeyEventProc(KEY_EVENT_RECORD ker);
	void MouseEventProc(MOUSE_EVENT_RECORD mer);
	void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr);

};

#endif