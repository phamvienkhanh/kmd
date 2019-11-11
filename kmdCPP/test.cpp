#include <conio.h>
#include <ctype.h>
#include <iostream>
#include <string>

int main( void )
{
   	int ch;
	std::string strcmd="";
   	_cputs( "Type 'Y' when finished typing keys: " );
   	do
   	{
      		ch = _getch();
		if(ch == 13)
		{
			std::cout <<std::endl<< strcmd << std::endl;
			strcmd = "";
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
			while(strcmd.size() > 0 && (strcmd.back() != ' '))
			{
				std::cout<<"\b \b";
				strcmd.pop_back();
			}
			strcmd += "recommend";
			std::cout << "recommend";
		}
		else
		{
			strcmd += ch;
			std::cout<< char(ch);
		}
   	} while(1);
 	
}