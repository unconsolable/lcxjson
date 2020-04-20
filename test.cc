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
	//以下添加数组
	switchElem.parse(str.c_str());
	bool succ;
	auto find2 = switchElem.findObjectByKey("profiles",succ);
	print_json(0, find2.findObjectByKey("list",succ).getArrayElement(2), true);
}