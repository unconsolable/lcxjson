#include "lcxjson.h"
#include <iostream>
#include <fstream>
#include <iomanip>
int main()
{
	// test using the setting json of Windows Terminal
	std::ifstream input("test.json");
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
	auto find2 = switchElem["profiles"];
	//输出结果
	print_json(0, find2["list"][2], true);
}