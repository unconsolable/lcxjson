#include "lcxjson.h"
#include <iostream>
#include <fstream>
#include <iomanip>
int main()
{
	// test using the setting json of Windows Terminal
	std::ifstream input("test");
	std::string ans; std::string tmp;
	while (input >> tmp)
	{
		ans += tmp + ' ';
	}
	elementValue rootElem;
	rootElem.parse(ans.c_str());
	std::string str; 
	rootElem.toString(str);
	elementValue switchElem;
	switchElem.parse(str.c_str());
	print_json(0, switchElem, true);
}