#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <string>
#include <fstream>
#include "json.hpp"
#include "Book.h"
#include "User.h"
using json = nlohmann::json;
using namespace std;
class File{
private:
    string filename;
public:
    File();
    static void wirteUsers(vector<User> user);
    static json toJson(User u);
    static json toJson(Book book);
    static Book toBook(json j);
    static User toUser(json j);
    static void wirteBooks(vector<Book> book);
    static vector<Book> readBooks();
    static vector<User> readUsers();
};
#endif