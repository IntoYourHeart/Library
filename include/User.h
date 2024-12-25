#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Book.h"
using namespace std;

class User
{
	string name;
	string password;
	bool isAdmin;
	unordered_map<string,bool> book;
public:
	User();
	User(string name, string password, bool isAdmin);
	string getPassword();
	bool isEmpty();
	bool getIsAdmin() const;
	void setName(string name);
	string getName() const;
	void addOrSetBook(string name, bool is);
	unordered_map<string,bool> getBook() const;
	void setPassword(string password);
	void resetPassword();
	void deleteBookByName(string na);
	void display() const;
};
#endif