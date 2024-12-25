#include <iostream>
#include <string>
#include <fstream>
#include "json.hpp"
#include "File.h"
#include "Book.h"
using json = nlohmann::json;
using namespace std;
File::File() {}

json File::toJson(Book book)
{
    return json{{"name", book.getName()}, {"ISBN", book.getISBN()}, {"ISSN", book.getISSN()}, {"author", book.getAuthor()}, \
    {"publish", book.getPublish()}, {"price", book.getPrice()}, {"introduction", book.getIntroduction()}, {"year", book.getYear()}, \
    {"month", book.getMonth()},{"rented_num", book.getRentedNum()},{"rent_time",book.getRentTime()},{"num",book.getNum()}};
}

json File::toJson(User user)
{
    unordered_map<string,bool> m = user.getBook();
    json j = json::array();
    for(auto v:m){
        j.push_back({{"name",v.first},{"is_renting",v.second}});
    }
    return json{{"name", user.getName()}, {"password", user.getPassword()}, {"isAdmin", user.getIsAdmin()},{"book",j}};
}

Book File::toBook(json j)
{
    return Book(j.at("name").get<string>(), j.at("ISBN").get<string>(), j.at("ISSN").get<string>(), j.at("author").get<string>(),
                j.at("publish").get<string>(), j.at("price").get<double>(), j.at("introduction").get<string>(), j.at("year").get<int>(),
                j.at("month").get<int>(),j.at("rented_num").get<int>(),j.at("rent_time").get<int>(),j.at("num").get<int>());
}
User File::toUser(json j)
{
    json book = j.at("book").get<json>();
    User u(j.at("name").get<string>(), j.at("password").get<string>(), j.at("isAdmin").get<bool>());
    for(const json& v:book){
        u.addOrSetBook(v.at("name").get<string>(),v.at("is_renting").get<bool>());
    }
    return u;
}

void File::wirteUsers(vector<User> user)
{
    ofstream out("user.dat", ios::out | ios::binary);
    json j = json::array();
    for (size_t i = 0; i < user.size(); i++)
    {
        j.push_back(toJson(user[i]));
    }
    out << j;
    out.close();
}

void File::wirteBooks(vector<Book> book)
{
    ofstream out("book.dat", ios::out | ios::binary);
    json j = json::array();
    for (size_t i = 0; i < book.size(); i++)
    {
        j.push_back(toJson(book[i]));
    }
    out << j;
    out.close();
}

vector<Book> File::readBooks()
{
    vector<Book> book;
    ifstream in("book.dat", ios::binary | ios::in);
    json j;
    char c;
    if (in >> c)
    {
        in.close();
        in = ifstream("book.dat", ios::binary | ios::in);
        in >> j;
        in.close();
        for (const auto &v : j)
        {
            book.push_back(toBook(v));
        }
        return book;
    }
    in.close();
    return vector<Book>();
}
vector<User> File::readUsers()
{
    vector<User> user;
    ifstream in("user.dat", ios::binary | ios::in);
    json j;
    char c;
    if (in >> c)
    {
        in.close();
        in = ifstream("user.dat", ios::binary | ios::in);
        in >> j;
        in.close();
        for (const auto &v : j)
        {
            user.push_back(toUser(v));
        }
        return user;
    }
    in.close();
    return user;
}