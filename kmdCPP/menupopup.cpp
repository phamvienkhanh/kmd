#include "menupopup.h"
#include "utilities.h"



void MenuPopup::SetListItems(std::vector<std::string> listItems)
{
	m_listItems.clear();
	m_listItems = listItems;
}

void MenuPopup::SetPosition(COORD pos)
{
	m_position = pos;
}


void MenuPopup::Show()
{
	m_isVisible   = true;
	m_saveCurrPos = Utilities::GetConsoleCursorPosition(m_hConsole);
	Utilities::ShowConsoleCursor(false, m_hConsole);

	for(short i=0; i < m_listItems.size(); i++)
	{
		COORD newLine = {m_position.X, m_position.Y + i};
		SetConsoleCursorPosition(m_hConsole,newLine);

		if(i == m_targetItem)
		{
			std::wcout << "--> " << m_listItems[i].c_str() << std::endl;
		}
		else
		{
			std::wcout << "    " << m_listItems[i].c_str() << std::endl;
		}

		SetConsoleCursorPosition(m_hConsole,m_position);
	}

}

void MenuPopup::Close()
{
	std::string whiteSpace(100, ' ');
	for(int i = 0; i < m_listItems.size(); i++)
	{
		COORD newtargetLine = {m_position.X, m_position.Y + i};
		SetConsoleCursorPosition(m_hConsole,newtargetLine);
		std::wcout << whiteSpace.c_str();
	}

	SetConsoleCursorPosition(m_hConsole,m_saveCurrPos);

	Utilities::ShowConsoleCursor(true, m_hConsole);
	m_targetItem   = 0;
	m_isVisible    = false;
}

void MenuPopup::HandleEventArrowKey(KEY_EVENT_RECORD keyEvent)
{
	if(m_isVisible)
	{
		COORD currtargetLine = {m_position.X, m_position.Y + m_targetItem};
		SetConsoleCursorPosition(m_hConsole,currtargetLine);
		std::wcout << "    ";

		if (keyEvent.wVirtualKeyCode == VK_UP)
		{
			if(--m_targetItem < 0)
				m_targetItem = (int)m_listItems.size() - 1;
		}
		else if (keyEvent.wVirtualKeyCode == VK_DOWN)
		{
			if(++m_targetItem >= (int)m_listItems.size())
				m_targetItem = 0;
		}
		else if(keyEvent.wVirtualKeyCode == VK_RETURN)
		{
			std::string senderItem = m_listItems[m_targetItem];

			Close();

			if(m_cbEventEnter != nullptr)
			{
				m_cbEventEnter(senderItem);
			}
			return;
		}

		COORD newtargetLine = {m_position.X, m_position.Y + m_targetItem};
		SetConsoleCursorPosition(m_hConsole,newtargetLine);

		std::wcout << "--> ";

		SetConsoleCursorPosition(m_hConsole,m_saveCurrPos);
	}
}


bool MenuPopup::IsVisiable()
{
	return m_isVisible;
}

void MenuPopup::SetCBEventEnter(std::function<void(std::string)> cbEventEnter)
{
	m_cbEventEnter = cbEventEnter;
}

MenuPopup::MenuPopup(HANDLE hConsole)
{
	m_isVisible    = false;
	m_position     = {0,0};
	m_targetItem   = 0;
	m_hConsole     = hConsole;
	m_cbEventEnter = nullptr;
	m_listItems.clear();
}


MenuPopup::~MenuPopup()
{

}