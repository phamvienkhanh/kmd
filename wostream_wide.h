#pragma once
#include <iostream>
#include <windows.h>

std::wostream& operator<< (std::wostream& o, const wchar_t * const s) throw(...)
{
	const UINT cp = CP_UTF8;
	bool error = false;
	if (s)
	{
		int bufferSize = WideCharToMultiByte(cp, 0, s, -1, NULL,          0, NULL, NULL);
		char* m = new char[bufferSize];
		//Remember the old console codepage.
		UINT oldcp = GetConsoleOutputCP();
		//Change it to what we want.
		SetConsoleOutputCP (cp);
		/*            */ WideCharToMultiByte(cp, 0, s, -1,    m, bufferSize, NULL, NULL);
		if (&o == &std::wcout) fwprintf(stdout, L"%S", m);
		else if (&o == &std::wcerr) fwprintf(stderr, L"%S", m);
		else error = true;
		//It would be nice to have this instead, but unfortunately it does not work.
		//o << m;
		//Revert to the old codepage.
		SetConsoleOutputCP (oldcp);
		delete[] m;
	}
	else
	{
		//If s == NULL we must not try to convert with WideCharToMultiByte or we will get junk.
		//We go straight to fwprintf instead that prints "(null)" - We do not need to convert "(null)"
		//because there are no Unicode characters in it ;)
		if (&o == &std::wcout) fwprintf(stdout, L"%s", s);
		else if (&o == &std::wcerr) fwprintf(stderr, L"%s", s);
		else error = true;
	}
	if (error) throw "Not wcout or wcerr";
	return o;
}
