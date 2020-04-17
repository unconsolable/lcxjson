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
	elementValue arrayItem;
	add.setBool(true);
	//以下添加数组
	arrayItem.parse("[]");
	arrayItem.addArrayElement(add);
	arrayItem.addArrayElement(add);
	switchElem.addObject("aa", add);
	switchElem.parse(str.c_str());
	switchElem.addObject("bb", arrayItem);
	auto find2 = switchElem.findObjectByKey("profiles");
	print_json(0, switchElem, true);
}