#include <iostream>
#include <string>
#include <algorithm>
#include "User.h"
#include "Book.h"
#include "md5.h"
using namespace std;

User::User() {}
User::User(string name, string password, bool isAdmin) : name(name), password(password), isAdmin(isAdmin) {}
void User::setName(string name)
{
	this->name = name;
}
void User::addOrSetBook(string name, bool is)
{
	book[name] = is;
}
unordered_map<string, bool> User::getBook() const
{
	return book;
}
bool User::isEmpty()
{
	for (auto v : book)
	{
		cout << v.second << endl;
		if (v.second)
		{
			return false;
		}
	}
	return true;
}
string User::getName() const
{
	return name;
}
string User::getPassword()
{
	return password;
}
bool User::getIsAdmin() const
{
	return isAdmin;
}
void User::setPassword(string password)
{
	this->password = md5(password);
}
void User::display() const
{
	cout << "********用户信息********" << endl;
	cout << "用户名：" << name << endl;
	cout << "用户类型：" << (isAdmin ? "管理员" : "读者") << endl;
	if (!isAdmin)
	{
		cout << "在借书目：" << endl;
		for (const auto &v : book)
		{
			if (v.second)
			{
				cout << "\t|- 《" << v.first << "》" << endl;
			}
		}
	}
}
void User::deleteBookByName(string na){
	book.erase(na);
}
void User::resetPassword()
{
	setPassword("123456");
}
