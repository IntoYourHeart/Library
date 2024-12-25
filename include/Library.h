#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Book.h"
#include "User.h"
#include "json.hpp"
#include "File.h"
using json = nlohmann::json;
using namespace std;
class Library
{
private:
    string TITLE;
    vector<Book> book;
    User user;
    vector<User> users;
public:
    Library(User user);
    void addBook();
    void addUser();
    void showAdminQueryMenu();
    void showAdminBookMenuAuthor(Book &boo);
    void showAdminBookMenu(Book& boo);
    Book& searchBookByName(const string &name);
    Book& searchBookByISBN(const string &ISBN);
    Book& searchBookByISSN(const string &ISSN);
    vector<Book*> searchBookByAuthor(const string &author);
    User& searchUserByName(const string &name);
    void sortByRentTime();
    void sortByPublishTime();
    void rentBook(Book& b);
    void deleteBook();
    bool isStandard(const string &str);
    int getSimilarNum(const User& us);
    void recommandBook();
    void backBook();
    void rank(vector<Book> book);
    void showQueryMenu();
    void showMainMenu();
};