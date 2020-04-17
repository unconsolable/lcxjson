#ifndef FACULTY_H
#define FACULTY_H
#include <string>
#include <vector>
#include "lcxjson.h"
enum personType
{
	FACULTY=20,
	STUDENT
};
class account
{
public:
	account() = default;
	account(const std::string& i, const std::string& pw, personType t) :id(i), passwd(pw), tp(t)
	{
	}
	void changePw(const std::string& newpw){passwd = newpw;}
	std::string getId() const { return id; }
	std::string getPw() const { return passwd; }
private:
	std::string id;
	std::string passwd;
	personType tp;
};
class faculty :public account
{
public:
	faculty() = default;
	faculty(const std::string& i, const std::string& pw, personType t) :account(i, pw, t) {}
};
class student :public account
{
public:
	student() = default;
	student(const std::string& i, const std::string& pw, personType t) :account(i, pw, t) {}
	void setName(const std::string& n) { name = n; }
	void showScore() const;
	void addScore(const std::string& name, const double& score);
	void deleteScore(const std::string& name);
	elementValue toJsonObject() const;
private:
	std::string name;
	std::vector<std::string> courseName;
	std::vector<double> courseScore;
};
#endif 