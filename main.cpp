#undef byte
#include <windows.h>
#include <iostream>
#include <regex>
#include "Library.h"
#include "User.h"
#include "Book.h"
#include "json.hpp"
#include "md5.h"
#include <fstream>
using namespace std;
using json = nlohmann::json;

string TITLE = R"(
_|        _|  _|
_|            _|_|_|    _|  _|_|    _|_|_|  _|  _|_|  _|    _|
_|        _|  _|    _|  _|_|      _|    _|  _|_|      _|    _|
_|        _|  _|    _|  _|        _|    _|  _|        _|    _|
_|_|_|_|  _|  _|_|_|    _|          _|_|_|  _|          _|_|_|
                                                            _|
                                                        _|_|
)";


vector<User> users = File::readUsers();
bool isStandard(const string &str)
{
    regex rule(R"((?=.*[a-zA-Z])(?=.*\d).+)");
    if (regex_match(str, rule))
    {
        return true;
    }
    else
    {
        return false;
    }
}
void addUser()
{
    string username, password,pass;
    bool isAdmin;
    int choice;
    system("cls");
        cout << TITLE << endl;
    cout << "****************" << endl;
    cout << "1. 读者" << endl;
    cout << "2. 管理员" << endl;
    cout << "****************" << endl;
    cout << "请输入用户类型：";
    while(true){
        cin >> choice;
        if(choice == 1 || choice == 2){
            break;
        }else{
            cout << "输入无效，请重新输入：";
        }
    }
    switch (choice)
    {
    case 1:
        isAdmin = false;
        break;
    case 2:
        cout << "请输入认证码：";
        while(true){
            cin >> pass; 
            if(pass == "666666"){
                isAdmin = true;
                break;
            }else{
                cout << "认证码错误，请重新输入：";
            }
        }
    }
    cout << "请输入用户名:";
    cin >> username;
    while(find_if(users.begin(),users.end(),[username](const User& u){return u.getName() == username;}) != users.end()){
        cout << "用户名已存在，请重新输入：";
        cin >> username;
    }
    while (true)
    {
        cout << "请输入密码:";
        cin >> password;
        if (password.size() > 8 && isStandard(password))
        {
            cout << "注册成功,已为您登录\n";
            ofstream out("login.dat");
            json j{{"name",username},{"password",md5(password)}};
            out << j;
            out.close();
            User acc(username, md5(password), isAdmin);
            users.push_back(acc);
            File::wirteUsers(users);
            Library lib(acc);
            lib.showMainMenu();
            break;
        }
    }
}
void login()
{
    string username, password;
    while (true)
    {
        cout << "请输入用户名:";
        cin >> username;
        if (find_if(users.begin(), users.end(), [username](User u)
                    { return u.getName() == username; }) != users.end())
        {
            break;
        }else if(username == "0"){
            return;
        }
        else
        {
            cout << "用户名不存在,请重新输入(输入0返回上一级目录):";
        }
    }
    auto it = find_if(users.begin(), users.end(), [username](User u)
                      { return u.getName() == username; });
    while (true)
    {
        cout << "请输入密码:";
        cin >> password;
        if (md5(password) == it->getPassword())
        {
            cout << "登录成功\n" << endl;
            ofstream out("login.dat");
            json j{{"name",username},{"password",it->getPassword()}};
            out << j;
            out.close();
            User user(username, md5(it->getPassword()), it->getIsAdmin());
            Library lib(user);
            lib.showMainMenu();
            break;
        }
    }
}
void showLoginMenu()
{
    int choice;
    ifstream in("login.dat");
    char c;
    in >> c;
    in.close();
    if(c){
        ifstream i("login.dat");
        json js;
        i >> js;
        i.close();
        string us = js.at("name").get<string>();
        auto it = find_if(users.begin(),users.end(),[us](User u){return u.getName() == us;});
        User u;
        u = *it;
        if(js.at("password").get<string>() != it->getPassword()){
            ofstream out("login.dat");
            out << "";
            out.close();
            cout << "检测到更改密码，请重新登录！" << endl;
        }else{
            Library lib(u);
            cout << "检测到您已登录，欢迎进入！" << endl;
            system("pause");
            lib.showMainMenu();
        }
    }
    do
    {
        users = File::readUsers();
        system("cls");
        cout << TITLE << endl;
        cout << "\n--- 您还没有登录，请选择： ---\n";
        cout << "1. 登录\n";
        cout << "2. 注册\n";
        cout << "3. 退出系统\n";
        cout << "请选择: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            login();
            break;
        case 2:
            addUser();
            break;
        case 3:
            cout << "已退出系统，欢迎再次使用！\n";
            system("cls");
            break;
        default:
            cout << "无效的选择, 请重新输入：\n";
            break;
        }
    } while (choice != 3);
}

int main()
{
    SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8
    SetConsoleCP(65001);       // 设置控制台输入为 UTF-8
    showLoginMenu();
    system("pause");
    return 0;
}
