#include "stdafx.h"

class MenuPopup
{

public:
	void SetListItems(std::vector<std::string> listItems);
	void SetPosition(COORD pos);
	void Show();
	bool IsVisiable();
	void Close();
	void HandleEventArrowKey(KEY_EVENT_RECORD keyEvent);

	void SetCBEventEnter(std::function<void(std::string)> cbEventEnter);

private:
	HANDLE	m_hConsole;
	COORD 	m_position;
	COORD 	m_saveCurrPos;
	int     m_targetItem;
	bool    m_isVisible;
	std::vector<std::string> m_listItems;

	std::function<void(std::string)> m_cbEventEnter;


public:
	MenuPopup(HANDLE hConsole);
	~MenuPopup();
};