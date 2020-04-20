#include "lcxjson.h"
#include "faculty.h"
#include <iostream>
#include <map>
#include <fstream>

#define SAVE_CHANGES()                                              \
	do                                                              \
	{                                                               \
		newRootElem.parse("{}");                                    \
		elementValue newFacultyValue;                               \
		newFacultyValue.parse("{}");                                \
		elementValue newStuAccValue;                                \
		newStuAccValue.parse("{}");                                 \
		elementValue newStuValue;                                   \
		newStuValue.parse("{}");                                    \
		for (const auto &curFac : storeFaculty)                     \
		{                                                           \
			elementValue newPw;                                     \
			newPw.setString(curFac.getPw());                        \
			newFacultyValue.addObject(curFac.getId(), newPw);       \
		}                                                           \
		for (const auto &curStu : storeStudent)                     \
		{                                                           \
			elementValue newPw, newCurStuValue;                     \
			newPw.setString(curStu.getPw());                        \
			newStuAccValue.addObject(curStu.getId(), newPw);        \
			newCurStuValue = curStu.toJsonObject();                 \
			newStuValue.addObject(curStu.getId(), newCurStuValue);  \
		}                                                           \
		elementValue newAccountValue;                               \
		newAccountValue.parse("{}");                                \
		newAccountValue.addObject("faculty", newFacultyValue);      \
		newAccountValue.addObject("student", newStuAccValue);       \
		newRootElem.addObject("account", newAccountValue);          \
		newRootElem.addObject("studata", newStuValue);              \
		std::string ans;                                            \
		newRootElem.toString(ans);                                  \
		std::ofstream out(filenm, std::ios::out | std::ios::trunc); \
		out << ans;                                                 \
		return 0;                                                   \
	} while (0)

int main()
{
	std::string filenm;
	std::cout << "File Location?";
	std::cin >> filenm;
	std::ifstream source(filenm);
	std::string ans;
	std::string tmp;
	while (source >> tmp)
	{
		ans += tmp + ' ';
	}
	source.close();
	elementValue rootElem;
	rootElem.parse(ans.c_str());
	elementValue newRootElem;
	//convert json into vector
	std::vector<faculty> storeFaculty;
	std::vector<student> storeStudent;
	std::map<std::string, double> stuid2index;
	std::map<std::string, double> facid2index;
	elementValue facultyInfo = rootElem.getObjectValue(0).getObjectValue(0);
	elementValue stuAccInfo = rootElem.getObjectValue(0).getObjectValue(1);
	elementValue stuInfo = rootElem.getObjectValue(1);
	//convert teacher info
	for (size_t cur = 0; cur < facultyInfo.getObjectSize(); ++cur)
	{
		facid2index.insert(std::make_pair(facultyInfo.getObjectKey(cur), cur));
		storeFaculty.emplace_back(facultyInfo.getObjectKey(cur), facultyInfo.getObjectValue(cur).getString(), FACULTY);
	}
	//convert student info
	for (size_t cur = 0; cur < stuAccInfo.getObjectSize(); ++cur)
	{
		std::string key = stuAccInfo.getObjectKey(cur);
		bool succ;
		elementValue correInfo = stuInfo.findObjectByKey(key, succ);
		if (succ)
		{
			stuid2index.insert(std::make_pair(key, cur));
			student newstu(key, stuAccInfo.getObjectValue(cur).getString(), STUDENT);
			newstu.setName(correInfo.getObjectValue(0).getString());
			for (size_t curAddScore = 0; curAddScore < correInfo.getObjectValue(1).getArraySize(); ++curAddScore)
			{
				newstu.addScore(correInfo.getObjectValue(1).getArrayElement(curAddScore).getString(), correInfo.getObjectValue(2).getArrayElement(curAddScore).getNumber());
			}
			storeStudent.push_back(newstu);
		}
	}
	//login process
	std::string id, curpw;
	std::cout << "ID:";
	std::cin >> id;
	std::cout << "Password:";
	std::cin >> curpw;
	if (id == "1000" && curpw == "sa")
	{
		//sa mode
		std::cout << "SU MODE\n";
		std::string hintStr = "su@" + filenm;
		while (1)
		{
			std::cout << hintStr << "#";
			std::string commandStr, IdStr;
			std::cin >> commandStr >> IdStr;
			if (commandStr == "add")
			{
				std::cout << "Faculty Press F, Student Press S:\n";
				char addTypeRes;
				std::cin >> addTypeRes;
				if (addTypeRes == 'F')
				{
					std::string addPw;
					std::cout << "Password:\n";
					std::cin >> addPw;
					faculty toAddFaculty(IdStr, addPw, FACULTY);
					storeFaculty.push_back(toAddFaculty);
					facid2index[toAddFaculty.getId()]=storeFaculty.size()-1;
				}
				else if (addTypeRes == 'S')
				{
					std::string addPw, addNm;
					std::cout << "Password:\n";
					std::cin >> addPw;
					std::cout << "Name:\n";
					std::cin >> addNm;
					student toAddStudent(IdStr, addPw, FACULTY);
					toAddStudent.setName(addNm);
					storeStudent.push_back(toAddStudent);
					stuid2index[toAddStudent.getId()]=storeStudent.size()-1;
				}
				else
				{
					std::cout << "Invaild command.\n";
				}
			}
			else if (commandStr == "delete")
			{
				std::cout << "Faculty Press F, Student Press S:\n";
				char delTypeRes;
				std::cin >> delTypeRes;
				if (delTypeRes == 'F')
				{
					size_t index;
					try
					{
						index = facid2index.at(IdStr);
					}
					catch (const std::out_of_range &err)
					{
						std::cout << err.what();
						continue;
					}
					storeFaculty.erase(storeFaculty.begin() + index, storeFaculty.begin() + index + 1);
				}
				else if (delTypeRes == 'S')
				{
					size_t index;
					try
					{
						index = stuid2index.at(IdStr);
					}
					catch (const std::out_of_range &err)
					{
						std::cout << err.what();
						continue;
					}
					storeStudent.erase(storeStudent.begin() + index, storeStudent.begin() + index + 1);
				}
				else
				{
					std::cout << "Invaild command.\n";
				}
			}
			else if (commandStr == "change")
			{
				std::cout << "Faculty Press F, Student Press S:\n";
				char chgTypeRes;
				std::cin >> chgTypeRes;
				if (chgTypeRes == 'F')
				{
					std::string newPw;
					size_t index;
					try
					{
						index = facid2index.at(IdStr);
					}
					catch (const std::out_of_range &err)
					{
						std::cout << err.what();
						continue;
					}
					std::cout << "New Password:\n";
					std::cin >> newPw;
					storeFaculty[index].changePw(newPw);
				}
				else if (chgTypeRes == 'S')
				{
					std::string newPw;
					size_t index;
					try
					{
						index = stuid2index.at(IdStr);
					}
					catch (const std::out_of_range &err)
					{
						std::cout << err.what();
						continue;
					}
					std::cout << "New Password:\n";
					std::cin >> newPw;
					storeStudent[index].changePw(newPw);
				}
				else
				{
					std::cout << "Invaild command.\n";
				}
			}
			else if (commandStr == "exit")
			{
				SAVE_CHANGES();
			}
			else
			{
				std::cout << "Invaild command\n";
			}
		}
	}
	else
	{
		bool succ;
		elementValue passwd = facultyInfo.findObjectByKey(id, succ);
		if (passwd.getString() == curpw)
		{
			//faculty mode
			std::cout << "Welcome " << id << '\n';
			std::string hintStr = id + '@' + filenm;
			while (1)
			{
				std::cout << hintStr << "$";
				std::string commandStr, stuIdStr;
				size_t index;
				std::cin >> commandStr >> stuIdStr;
				if (commandStr != "exit")
				{
					try
					{
						index = stuid2index.at(stuIdStr);
					}
					catch (const std::out_of_range &err)
					{
						std::cout << err.what();
						continue;
					}
				}
				if (commandStr == "add")
				{
					std::string addCourseName;
					double addCourseMark;
					std::cout << "CourseName?";
					std::cin >> addCourseName;
					std::cout << "CourseMark?";
					std::cin >> addCourseMark;
					storeStudent[index].addScore(addCourseName, addCourseMark);
				}
				else if (commandStr == "delete")
				{
					std::string delCourseName;
					std::cout << "CourseName?";
					std::cin >> delCourseName;
					try
					{
						storeStudent[index].deleteScore(delCourseName);
					}
					catch (const std::exception &e)
					{
						std::cout << e.what();
					}
				}
				else if (commandStr == "show")
				{
					storeStudent[index].showScore();
				}
				else if (commandStr == "exit")
				{
					SAVE_CHANGES();
				}
				else
				{
					std::cout << "Invaild command.\n";
				}
			}
		}
		else
		{
			passwd = stuAccInfo.findObjectByKey(id, succ);
			if (passwd.getString() == curpw)
			{
				//student mode
				auto indexInVector = stuid2index[id];
				storeStudent[indexInVector].showScore();
				return 0;
			}
			else
			{
				std::cout << "Invalid password.";
				return 0;
			}
		}
	}
}