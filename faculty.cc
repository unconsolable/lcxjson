#include "faculty.h"
#include <iostream>
#include <algorithm>

void student::showScore() const
{
	std::cout << "Score of " << name << "\n";
	for (size_t i = 0; i < courseName.size(); ++i)
	{
		std::cout << courseName[i] << ":" << courseScore[i] << '\n';
	}
}

void student::addScore(const std::string& name, const double& score)
{
	courseName.emplace_back(name);
	courseScore.emplace_back(score);
}

void student::deleteScore(const std::string& name)
{
	auto whereis = std::find(courseName.cbegin(), courseName.cend(), name);
	if (whereis == courseName.end())
	{
		throw std::out_of_range("Invaild courseName");
	}
	size_t index = whereis - courseName.cbegin();
	courseName.erase(whereis, whereis + 1);
	courseScore.erase(courseScore.cbegin() + index, courseScore.cbegin() + index + 1);
}

elementValue student::toJsonObject() const
{
	elementValue retValue;
	elementValue newName, newCourseNameArray, newCourseScoreArray;
	newName.setString(name);
	retValue.parse("{}");
	newCourseNameArray.parse("[]");
	newCourseScoreArray.parse("[]");
	for (const auto& curCourseName : courseName)
	{
		elementValue curCourseNameToJson;
		curCourseNameToJson.setString(curCourseName);
		newCourseNameArray.addArrayElement(curCourseNameToJson);
	}
	for (const auto& curCourseScore : courseScore)
	{
		elementValue curCourseScoreToJson;
		curCourseScoreToJson.setNumber(curCourseScore);
		newCourseScoreArray.addArrayElement(curCourseScoreToJson);
	}
	retValue.addObject("name", newName);
	retValue.addObject("courseName", newCourseNameArray);
	retValue.addObject("courseScore", newCourseScoreArray);
	return retValue;
}
