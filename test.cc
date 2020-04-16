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
	elementValue add;
	add.setBool(true);
	switchElem.parse(str.c_str());
	switchElem.addObject("aa", add);
	auto find2 = switchElem.findObjectByKey("profiles");
	print_json(0, find2, true);
}