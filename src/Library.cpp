#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include "Book.h"
#include "User.h"
#include "Library.h"
#include "json.hpp"
#include "File.h"
#include "md5.h"
using json = nlohmann::json;
using namespace std;
// 多本书
static Book null("NULL");
static User u("NULL", "NULL", "NULL");

Library::Library(User user)
{
    TITLE = R"(
_|        _|  _|
_|            _|_|_|    _|  _|_|    _|_|_|  _|  _|_|  _|    _|
_|        _|  _|    _|  _|_|      _|    _|  _|_|      _|    _|
_|        _|  _|    _|  _|        _|    _|  _|        _|    _|
_|_|_|_|  _|  _|_|_|    _|          _|_|_|  _|          _|_|_|
                                                            _|
                                                        _|_|
)";
    this->book = File::readBooks();
    this->users = File::readUsers();
    this->user = searchUserByName(user.getName());
}
bool Library::isStandard(const string &str)
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
void Library::addUser()
{
    string username, password, pass;
    bool isAdmin;
    int choice;
    system("cls");
    cout << TITLE << endl;
    cout << "****************" << endl;
    cout << "1. 读者" << endl;
    cout << "2. 管理员" << endl;
    cout << "****************" << endl;
    cout << "请输入用户类型：";
    while (true)
    {
        cin >> choice;
        if (choice == 1 || choice == 2)
        {
            break;
        }
        else
        {
            cout << "输入无效，请重新输入：";
        }
    }
    switch (choice)
    {
    case 1:
        isAdmin = false;
        break;
    case 2:
        isAdmin = true;
        break;
    }
    cout << "请输入用户名:";
    cin >> username;
    while (true)
    {
        cout << "请输入密码:";
        cin >> password;
        if (password.size() > 8 && isStandard(password))
        {
            ofstream out("login.dat");
            json j{{"name", username}, {"password", md5(password)}};
            out << j;
            out.close();
            system("cls");
            cout << "添加用户成功" << endl;
            User acc(username, md5(password), isAdmin);
            users.push_back(acc);
            File::wirteUsers(users);
            Library lib(acc);
            lib.showMainMenu();
            system("pause");
            break;
        }
    }
}
Book &Library::searchBookByName(const string &name)
{
    auto it = find_if(book.begin(), book.end(), [name](Book &b)
                      { return b.getName() == name; });
    if (it != book.end())
    {
        return *it;
    }
    else
    {
        system("cls");
        cout << "未查找到名为《" << name << "》的书籍" << endl;
        system("pause");

        return null;
    }
}
Book &Library::searchBookByISBN(const string &ISBN)
{
    auto it = find_if(book.begin(), book.end(), [ISBN](Book &b)
                      { return b.getISBN() == ISBN; });
    if (it != book.end())
    {
        return *it;
    }
    else
    {
        system("cls");

        cout << "未查找到ISBN为" << ISBN << "的书籍" << endl;
        system("pause");

        return null;
    }
}
Book &Library::searchBookByISSN(const string &ISSN)
{
    auto it = find_if(book.begin(), book.end(), [ISSN](Book &b)
                      { return b.getISSN() == ISSN; });
    if (it != book.end())
    {
        return *it;
    }
    else
    {
        system("cls");

        cout << "未查找到ISSN为" << ISSN << "的书籍" << endl;
        system("pause");

        return null;
    }
}
vector<Book *> Library::searchBookByAuthor(const string &author)
{
    vector<Book *> b;
    for (Book &bo : book)
    {
        if (bo.getAuthor() == author)
        {
            b.push_back(&bo);
        }
    }
    if (b.size() == 0)
    {
        system("cls");

        cout << "未查找到作者为" << author << "的书籍" << endl;
        system("pause");

        b.push_back(&null);
    }
    return b;
}
void Library::rank(vector<Book> book)
{
    sort(book.begin(), book.end(), [](Book &a, Book &b)
         {
             if(a.getYear()!=b.getYear())return a.getYear()>b.getYear();
             else{
                 return a.getMonth()>b.getMonth();
             } });
}
void Library::rentBook(Book &bk)
{
    int c;
    unordered_map<string, bool> b = user.getBook();
    string nam = bk.getName();
    if (bk.getName() != "NULL" && !bk.isRented() && (find_if(b.begin(), b.end(), [nam](const pair<string, bool> &p)
                                                             {
        if(p.first == nam && p.second == true){
            return true;
        }else{
            return false;
        } }) == b.end()))
    {
        cout << "是否借阅（1-是 2-否）：";
        while (true)
        {
            cin >> c;
            if (c == 1 || c == 2)
            {
                break;
            }
            else
            {
                cout << "输入无效，请重新输入：";
            }
        }
        string na;
        switch (c)
        {
        case 1:
            searchBookByName(bk.getName()).addRentedNum();
            searchBookByName(bk.getName()).addRentTime();
            na = user.getName();
            user.addOrSetBook(bk.getName(), true);
            find_if(users.begin(), users.end(), [na](User &u)
                    { return u.getName() == na; })
                ->addOrSetBook(bk.getName(), true);
            File::wirteUsers(users);
            File::wirteBooks(book);
            system("cls");
            cout << "借阅成功" << endl;
            system("pause");
            break;
        case 2:
            break;
        }
    }
    else
    {
        system("pause");
    }
}
void Library::showQueryMenu()
{
    int choice, c;
    string temp, temp_choice, na;
    Book bk;
    unordered_map<string, bool> b;
    vector<Book *> temp_book, rentable_book;
    do
    {
        system("cls");
        cout << TITLE << endl;
        cout << "*****请选择查询方式*****\n";
        cout << "1. 图书名称\n";
        cout << "2. ISBN\n";
        cout << "3. ISSN\n";
        cout << "4. 作者\n";
        cout << "5. 返回上级目录\n";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "请输入图书名称:";
            cin >> temp;
            bk = searchBookByName(temp);
            if (bk.getName() != "NULL")
            {
                bk.display();
                rentBook(bk);
            }
            break;
        case 2:
            cout << "请输入ISBN码:";
            cin >> temp;
            bk = searchBookByISBN(temp);
            if (bk.getName() != "NULL")
            {
                bk.display();
                rentBook(bk);
            }
            break;
        case 3:
            cout << "请输入ISSN码:";
            cin >> temp;
            bk = searchBookByISSN(temp);
            if (bk.getName() != "NULL")
            {
                bk.display();
                rentBook(bk);
            }
            break;
        case 4:
            cout << "请输入作者名:";
            cin >> temp;
            temp_book = searchBookByAuthor(temp);
            if (temp_book[0]->getName() != "NULL")
            {
                cout << "图书馆共收录该作者图书" << temp_book.size() << "本书" << endl;
                b = user.getBook();
                for (int i = 0; i < temp_book.size(); i++)
                {
                    cout << "第" << (i + 1) << "本" << endl;
                    temp_book[i]->display();
                    na = temp_book[i]->getName();
                    if ((!temp_book[i]->isRented()) && (find_if(b.begin(), b.end(), [na](const pair<string, bool> &p)
                                                                { return (p.first == na && p.second == true); }) == b.end()))
                    {
                        rentable_book.push_back(temp_book[i]);
                    }
                }
                cout << endl;
                if (rentable_book.size() > 0)
                {
                    cout << "其中共" << rentable_book.size() << "本可借阅,是否需要借阅（y/Y-是,n/N-否）：";
                    cin >> temp_choice;
                    while (!(temp_choice == "y" || temp_choice == "Y" || temp_choice == "n" || temp_choice == "N"))
                    {
                        cout << "输入错误，请重新输入：";
                        cin >> temp_choice;
                    }
                    if (temp_choice == "y" || temp_choice == "Y")
                    {
                        system("cls");
                        cout << TITLE << endl;
                        for (int i = 0; i < rentable_book.size(); i++)
                        {
                            cout << (i + 1) << ". 《" << rentable_book[i]->getName() << "》" << endl;
                        }
                        cout << "请选择要借阅的书籍：";
                        cin >> c;
                        while (!(c >= 1 && c <= rentable_book.size()))
                        {
                            cout << "输入有误，请重新输入：";
                            cin >> c;
                        }
                        user.addOrSetBook(rentable_book[c - 1]->getName(), true);
                        searchUserByName(user.getName()).addOrSetBook(rentable_book[c - 1]->getName(), true);
                        rentable_book[c - 1]->addRentedNum();
                        File::wirteBooks(book);
                        File::wirteUsers(users);
                        system("cls");
                        cout << "借阅成功！" << endl;
                        system("pause");
                    }
                }
                else
                {
                    system("pause");
                }
            }
            break;
        case 5:
            break;
        default:
            cout << "输入无效，请重新输入：";
            break;
        }
    } while (choice != 5);
}

void Library::backBook()
{
    vector<string> bs;
    int choice;
    if (user.isEmpty())
    {
        system("cls");
        cout << "无可归还书目" << endl;
        system("pause");
        return;
    }
    for (auto v : user.getBook())
    {
        if (v.second)
        {
            bs.push_back(v.first);
        }
    }
    system("cls");
    cout << TITLE << endl;
    cout << "****************" << endl;
    for (int i = 1; i < bs.size() + 1; i++)
    {
        cout << i << ". " << bs[i - 1] << endl;
    }
    cout << "****************" << endl;
    cout << "请选择你要归还的书:";
    while (true)
    {
        cin >> choice;
        if (choice >= 1 && choice <= bs.size() + 1)
        {
            break;
        }
        else
        {
            cout << "输入无效，请重新输入：";
        }
    }
    searchBookByName(bs[choice - 1]).reduceRentedNum();
    string na = bs[choice - 1];
    string un = user.getName();
    user.addOrSetBook(na, false);
    auto i = find_if(users.begin(), users.end(), [un](const User &u)
                     { return u.getName() == un; });
    i->addOrSetBook(na, false);
    File::wirteBooks(book);
    File::wirteUsers(users);
    system("cls");
    cout << "归还成功" << endl;
    system("pause");
}
void Library::showAdminBookMenu(Book &boo)
{
    int ch, year, month;
    double price;
    string c, na;
    system("cls");
    cout << TITLE << endl;
    boo.adminDisplay();
    cout << "**********菜 单**********" << endl;
    cout << "1. 更改图书" << endl;
    cout << "2. 删除图书" << endl;
    cout << "3. 返回" << endl;
    cout << "*************************" << endl;
    cout << "\n请输入您的选择：";
    cin >> ch;
    while (!(ch == 1 || ch == 2 || ch == 3))
    {
        cout << "输入有误，请重新输入：";
        cin >> ch;
    }
    switch (ch)
    {
    case 1:
        cout << "请输入图书名称(不更改请输入0)：";
        cin >> c;
        if (c != "0")
        {
            boo.setName(c);
        }
        c = "";
        cout << "请输入图书ISBN码(不更改请输入0)：";
        while (true)
        {
            cin >> c;
            if (c.size() == 13 || c == "0")
            {
                break;
            }
            else
            {
                cout << "请输入标准13位ISBN码(不更改请输入0)：";
            }
        }
        if (c != "0")
        {
            boo.setISBN(c);
        }
        c = "";
        cout << "请输入图书ISSN码(不更改请输入0)：";
        while (true)
        {
            cin >> c;
            if (c.size() == 8 || c == "0")
            {
                break;
            }
            else
            {
                cout << "请输入标准8位ISSN码(不更改请输入0)：";
            }
        }
        if (c != "0")
        {
            boo.setISSN(c);
        }
        c = "";
        cout << "请输入图书作者(不更改请输入0)：";
        cin >> c;
        if (c != "0")
        {
            boo.setAuthor(c);
        }
        c = "";
        cout << "请输入图书出版社(不更改请输入0)：";
        cin >> c;
        if (c != "0")
        {
            boo.setPublish(c);
        }
        cout << "请输入图书出版时间（分别输入年和月，用空格隔开）(不更改请输入0)：";
        cin >> year;
        if (year != 0)
        {
            while (true)
            {
                cin >> month;
                if (month >= 0 && month <= 12)
                {
                    break;
                }
                else
                {
                    cout << "请输入正确的月份：";
                }
            }
            boo.setYear(year);
            boo.setMonth(month);
        }
        cout << "请输入图书价格(不更改请输入0)：";
        cin >> price;
        if (price != 0)
        {
            boo.setPrice(price);
        }
        c = "";
        cout << "请输入图书简介(不更改请输入0)：";
        cin >> c;
        if (c != "0")
        {
            boo.setIntroduction(c);
        }
        cout << "请输入图书库存(不更改请输入0)：";
        cin >> month;
        if (month != 0 && month < boo.getRentedNum())
        {
            while (month < boo.getRentedNum())
            {
                cout << "该书目前仍有" << boo.getRentedNum() << "本在借，不能设置库存为" << month << "，请重新输入：";
                cin >> month;
            }
        }
        if (month != 0)
        {
            boo.setNum(month);
        }
        File::wirteBooks(book);
        system("cls");
        cout << "书籍信息已更新！" << endl;
        system("pause");
        break;
    case 2:
        if (boo.getRentedNum() == 0)
        {
            na = boo.getName();
            book.erase(remove_if(book.begin(), book.end(), [na](Book &b)
                                 { return b.getName() == na; }),
                       book.end());
            for (User &v : users)
            {
                v.deleteBookByName(boo.getName());
            }
            File::wirteBooks(book);
            File::wirteUsers(users);
            system("cls");
            cout << "删除成功！" << endl;
            system("pause");
        }
        else
        {
            system("cls");
            cout << "该书仍有在借图书，无法删除" << endl;
            system("pause");
        }
        break;
    case 3:
        break;
    }
}
void Library::showAdminBookMenuAuthor(Book &boo)
{
    int ch, year, month;
    double price;
    string c, na;
    boo.adminDisplay();
    cout << "**********菜 单**********" << endl;
    cout << "1. 更改图书" << endl;
    cout << "2. 删除图书" << endl;
    cout << "3. 返回" << endl;
    cout << "*************************" << endl;
    cout << "\n请输入您的选择：";
    cin >> ch;
    while (!(ch == 1 || ch == 2 || ch == 3))
    {
        cout << "输入有误，请重新输入：";
        cin >> ch;
    }
    switch (ch)
    {
    case 1:
        cout << "请输入图书名称(不更改请输入0)：";
        cin >> c;
        if (c != "0")
        {
            boo.setName(c);
        }
        c = "";
        cout << "请输入图书ISBN码(不更改请输入0)：";
        while (true)
        {
            cin >> c;
            if (c.size() == 13 || c == "0")
            {
                break;
            }
            else
            {
                cout << "请输入标准13位ISBN码(不更改请输入0)：";
            }
        }
        if (c != "0")
        {
            boo.setISBN(c);
        }
        c = "";
        cout << "请输入图书ISSN码(不更改请输入0)：";
        while (true)
        {
            cin >> c;
            if (c.size() == 8 || c == "0")
            {
                break;
            }
            else
            {
                cout << "请输入标准8位ISSN码(不更改请输入0)：";
            }
        }
        if (c != "0")
        {
            boo.setISSN(c);
        }
        c = "";
        cout << "请输入图书作者(不更改请输入0)：";
        cin >> c;
        if (c != "0")
        {
            boo.setAuthor(c);
        }
        c = "";
        cout << "请输入图书出版社(不更改请输入0)：";
        cin >> c;
        if (c != "0")
        {
            boo.setPublish(c);
        }
        cout << "请输入图书出版时间（分别输入年和月，用空格隔开）(不更改请输入0)：";
        cin >> year;
        if (year != 0)
        {
            while (true)
            {
                cin >> month;
                if (month >= 0 && month <= 12)
                {
                    break;
                }
                else
                {
                    cout << "请输入正确的月份：";
                }
            }
            boo.setYear(year);
            boo.setMonth(month);
        }
        cout << "请输入图书价格(不更改请输入0)：";
        cin >> price;
        if (price != 0)
        {
            boo.setPrice(price);
        }
        c = "";
        cout << "请输入图书简介(不更改请输入0)：";
        cin >> c;
        if (c != "0")
        {
            boo.setIntroduction(c);
        }
        cout << "请输入图书库存(不更改请输入0)：";
        cin >> month;
        if (month != 0 && month < boo.getRentedNum())
        {
            while (month < boo.getRentedNum())
            {
                cout << "该书目前仍有" << boo.getRentedNum() << "本在借，不能设置库存为" << month << "，请重新输入：";
                cin >> month;
            }
        }
        if (month != 0)
        {
            boo.setNum(month);
        }
        File::wirteBooks(book);
        system("cls");
        cout << "书籍信息已更新！" << endl;
        system("pause");
        break;
    case 2:
        if (boo.getRentedNum() == 0)
        {
            na = boo.getName();
            book.erase(remove_if(book.begin(), book.end(), [na](Book &b)
                                 { return b.getName() == na; }),
                       book.end());
            for (User &v : users)
            {
                v.deleteBookByName(boo.getName());
            }
            File::wirteBooks(book);
            File::wirteUsers(users);
            system("cls");
            cout << "删除成功！" << endl;
            system("pause");
        }
        else
        {
            system("cls");
            cout << "该书仍有在借图书，无法删除" << endl;
            system("pause");
        }
        break;
    case 3:
        break;
    }
}
void Library::showAdminQueryMenu()
{
    int choice, n = 0, nn = 0, ch;
    string temp;
    string c;
    double price;
    int year, month;
    vector<Book *> t_book;
    Book *boo;
    do
    {
        system("cls");
        cout << TITLE << endl;
        cout << "*****请选择查询方式*****\n";
        cout << "1. 陈列所有图书信息\n";
        cout << "2. 图书名称\n";
        cout << "3. ISBN\n";
        cout << "4. ISSN\n";
        cout << "5. 作者\n";
        cout << "6. 返回上级目录\n";
        cin >> choice;

        switch (choice)
        {
        case 1:
            book = File::readBooks();
            n = 0;
            for (Book b : book)
            {
                n += (b.getNum() - b.getRentedNum());
                nn += b.getNum();
            }
            system("cls");
            cout << TITLE << endl;
            cout << "目前图书馆共有" << nn << "本书，其中" << n << "本可借" << endl;
            for (Book v : book)
            {
                v.adminDisplay();
            }
            system("pause");
            break;
        case 2:
            cout << "请输入图书名称:";
            cin >> temp;
            boo = &searchBookByName(temp);
            if (boo->getName() == "NULL")
            {
                break;
            }
            system("cls");
            cout << TITLE << endl;
            showAdminBookMenu(*boo);
            break;
        case 3:
            cout << "请输入ISBN码:";
            cin >> temp;
            boo = &searchBookByISBN(temp);
            if (boo->getName() == "NULL")
            {
                break;
            }
            showAdminBookMenu(*boo);
            break;
        case 4:
            cout << "请输入ISSN码:";
            cin >> temp;
            boo = &searchBookByISSN(temp);
            if (boo->getName() == "NULL")
            {
                break;
            }
            showAdminBookMenu(*boo);
            break;
        case 5:
            cout << "请输入作者名:";
            cin >> temp;
            t_book = searchBookByAuthor(temp);
            if (t_book[0]->getName() != "NULL")
            {
                system("cls");
                cout << TITLE << endl;
                cout << "图书馆共收录该作者图书" << t_book.size() << "本书" << endl;
                for (int i = 0; i < t_book.size(); i++)
                {
                    cout << "第" << (i + 1) << "本" << endl;
                    showAdminBookMenuAuthor(*t_book[i]);
                }
            }
        case 6:
            break;
        default:
            cout << "输入无效，请重新输入！\n";
            break;
        }
    } while (choice != 6);
}
void Library::sortByRentTime()
{
    sort(book.begin(), book.end(), [](const Book &a, const Book &b)
         { return a.getRentTime() > b.getRentTime(); });
    system("cls");
    cout << TITLE << endl;
    cout << "********借阅排行榜前十图书********" << endl;
    if (book.size() >= 10)
    {
        for (int i = 0; i < 10; i++)
        {
            cout << "-----------第" << (i + 1) << "名" << "-----------" << endl;
            book[i].display();
            cout << endl;
        }
        system("pause");
    }
    else
    {
        for (int i = 0; i < book.size(); i++)
        {
            cout << "-----------第" << (i + 1) << "名" << "-----------" << endl;
            book[i].display();
            cout << endl;
        }
        system("pause");
    }
}
void Library::sortByPublishTime()
{
    sort(book.begin(), book.end(), [](const Book &a, const Book &b)
         {
        if(a.getYear() == b.getYear()){
            return a.getMonth() > b.getMonth();
        }else{
            return a.getYear() > b.getYear();
        } });
    system("cls");
    cout << TITLE << endl;
    cout << "********出版时间排行榜前十图书********" << endl;
    if (book.size() >= 10)
    {
        for (int i = 0; i < 10; i++)
        {
            cout << "-----------第" << (i + 1) << "名" << "-----------" << endl;
            book[i].display();
            cout << endl;
        }
        system("pause");
    }
    else
    {
        for (int i = 0; i < book.size(); i++)
        {
            cout << "-----------第" << (i + 1) << "名" << "-----------" << endl;
            book[i].display();
            cout << endl;
        }
        system("pause");
    }
}
int Library::getSimilarNum(const User &us)
{
    unordered_map<string, bool> user_book = user.getBook();
    int times = 0;
    string book_name;
    unordered_map<string, bool> us_b = us.getBook();
    for (const auto &b : us_b)
    {
        book_name = b.first;
        if (find_if(user_book.begin(), user_book.end(), [book_name](const auto &a)
                    { return a.first == book_name; }) != user.getBook().end())
        {
            times++;
        }
    }
    return times;
}
void Library::recommandBook()
{
    system("cls");
    cout << TITLE << endl;
    int duzhe_num = 0;
    User b("NULL", "NULL", false);
    vector<Book> recommand_book;
    unordered_map<string, bool> rec_user_book, user_book;
    user_book = user.getBook();
    for (const User &v : users)
    {
        if (!v.getIsAdmin())
        {
            duzhe_num++;
        }
    }
    if (user.getBook().size() == 0)
    {
        system("cls");
        cout << TITLE << endl;
        cout << "查询不到您的借阅记录，无法推荐" << endl;
        system("pause");

        return;
    }
    else if (user.getBook().size() == book.size())
    {
        system("cls");
        cout << TITLE << endl;
        cout << "恭喜您达成成就 #阅书万卷# ，暂无可推荐书目" << endl;
        system("pause");
        return;
    }
    else if (duzhe_num <= 1)
    {
        system("cls");
        cout << TITLE << endl;
        cout << "注册读者数量过少，无法推荐" << endl;
        system("pause");
        return;
    }
    for (const User u : users)
    {
        if (u.getName() == user.getName())
        {
            continue;
        }
        if (getSimilarNum(u) >= 1 && getSimilarNum(u) > getSimilarNum(b) && u.getBook().size() > getSimilarNum(u))
        {
            b = u;
        }
    }
    if (b.getName() != "NULL")
    {
        rec_user_book = b.getBook();
        for (const pair<string, bool> &v : rec_user_book)
        {
            if (find_if(user_book.begin(), user_book.end(), [v](const auto &a)
                        { return a.first == v.first; }) == user_book.end())
            {
                recommand_book.push_back(searchBookByName(v.first));
            }
        }
        if (recommand_book.size() > 0)
        {
            system("cls");
            cout << TITLE << endl;
            cout << "为您推荐以下书目：" << endl;
            for (int i = 0; i < recommand_book.size(); i++)
            {
                cout << (i + 1) << ". 《" << recommand_book[i].getName() << "》" << endl;
            }
            // 未完待续（借阅等）
            system("pause");
        }
    }
    else
    {
        cout << "没有找到与您有相同爱好的读者或与您爱好相同的读者没有借阅其他书，无法推荐" << endl;
    }
}
void Library::addBook()
{
    string name, ISBN, ISSN, author, publish, introduction, c;
    double price;
    int year, month;
    system("cls");
    cout << TITLE << endl;
    cout << "请输入图书名称：";
    cin >> name;
    if (find_if(book.begin(), book.end(), [name](const Book &b)
                { return b.getName() == name; }) != book.end())
    {
        cout << "该书已经存在，是否增添库存（y/Y-是 n/N-否）：";
        cin >> c;
        while (!(c == "y" || c == "Y" || c == "n" || c == "N"))
        {
            cout << "输入有误，请重新输入：";
            cin >> c;
        }
        if (c == "y" || c == "Y")
        {
            searchBookByName(name).addNum();
            File::wirteBooks(book);
            cout << "增添成功，目前该书库存为" << searchBookByName(name).getNum() << "本" << endl;
            system("pause");
            return;
        }
        else
        {
            cout << "取消" << endl;
            system("pause");
            return;
        }
        return;
    }
    cout << "请输入图书ISBN码：";
    while (true)
    {
        cin >> ISBN;
        if (ISBN.size() == 13)
        {
            break;
        }
        else
        {
            cout << "请输入标准13位ISBN码：";
        }
    }
    cout << "请输入图书ISSN码：";
    while (true)
    {
        cin >> ISSN;
        if (ISSN.size() == 8)
        {
            break;
        }
        else
        {
            cout << "请输入标准8位ISSN码：";
        }
    }
    cout << "请输入图书作者：";
    cin >> author;
    cout << "请输入图书出版社：";
    cin >> publish;
    cout << "请输入图书出版时间（分别输入年和月，用空格隔开）：";
    cin >> year;
    while (true)
    {
        cin >> month;
        if (month >= 0 && month <= 12)
        {
            break;
        }
        else
        {
            cout << "请输入正确的月份：";
        }
    }
    cout << "请输入图书价格：";
    cin >> price;
    cout << "请输入图书简介：";
    cin >> introduction;
    cout << "****************\n";
    Book b(name, ISBN, ISSN, author, publish, price, introduction, year, month);
    book.push_back(b);
}

User &Library::searchUserByName(const string &name)
{
    auto it = find_if(users.begin(), users.end(), [name](const User &u)
                      { return u.getName() == name; });
    if (it != users.end())
    {
        return *it;
    }
    cout << "查找的用户不存在！" << endl;
    return u;
}
void Library::showMainMenu()
{
    ofstream out;
    int choice, c;
    string na, pass;
    if (!user.getIsAdmin())
    {
        do
        {
            cout.flush();
            system("cls");
            cout << TITLE << endl;
            cout << "********菜单*********\n";
            cout << "1. 查询/借阅图书\n";
            cout << "2. 归还图书\n";
            cout << "3. 图书推荐\n";
            cout << "4. 查看排行榜\n";
            cout << "5. 修改密码\n";
            cout << "6. 退出登录\n";
            cout << "7. 退出系统\n";
            cin >> choice;

            switch (choice)
            {
            case 1:
                showQueryMenu();
                break;
            case 2:
                backBook();
                break;
            case 3:
                recommandBook();
                break;
            case 4:
                system("cls");
                cout << TITLE << endl;
                cout << "1. 借阅排行榜" << endl;
                cout << "2. 出版时间排行榜" << endl;
                cout << "请输入要查询的排行榜：";
                while (true)
                {
                    cin >> c;
                    if (c == 1 || c == 2)
                    {
                        break;
                    }
                    else
                    {
                        cout << "输入有误，请重新输入：";
                    }
                }
                switch (c)
                {
                case 1:
                    sortByRentTime();
                    break;
                case 2:
                    sortByPublishTime();
                    break;
                }
                break;
            case 5:
                cout << "请输入您要修改的新密码：";
                cin >> pass;
                while (!isStandard(pass))
                {
                    cout << "密码不符合规范，请重新输入：";
                    cin >> pass;
                }
                user.setPassword(pass);
                searchUserByName(user.getName()).setPassword(pass);
                File::wirteUsers(users);
                system("cls");
                cout << TITLE << endl;
                cout << "修改成功,请重新登录！" << endl;
                system("pause");
                out = ofstream("login.dat");
                out << "";
                out.close();
                break;
            case 6:
                out = ofstream("login.dat");
                out << "";
                out.close();
                cout << "已为您退出登录" << endl;
                system("pause");
                break;
            case 7:
                system("cls");
                cout << "已退出系统，欢迎再次使用！\n";
                system("pause");
                exit(0);
            default:
                cout << "输入无效，请重新输入！\n";
                break;
            }
        } while (choice != 6 && choice != 5);
    }
    else
    {
        do
        {
            cout.flush();
            system("cls");
            cout << TITLE << endl;
            cout << "********菜 单*********\n";
            cout << "1. 增添图书\n";
            cout << "2. 查询/更改/删除图书\n";
            cout << "3. 查询用户\n";
            cout << "4. 重置用户密码\n";
            cout << "5. 更改用户用户名\n";
            cout << "6. 更改密码\n";
            cout << "7. 删除用户\n";
            cout << "8. 查看排行榜\n";
            cout << "9. 退出登录\n";
            cout << "10. 退出系统\n";
            cout << "**********************";
            cout << "\n请选择：";
            cin >> choice;
            ofstream out;
            switch (choice)
            {
            case 1:
                cout << "请输入要添加的图书数目：";
                int n;
                cin >> n;
                while (n--)
                {
                    addBook();
                }
                File::wirteBooks(book);
                break;
            case 2:
                showAdminQueryMenu();
                break;
            case 3:
                cout << "请输入待查询的用户名称：";
                cin >> na;
                system("cls");
                cout << TITLE << endl;
                if (searchUserByName(na).getName() != "NULL")
                {
                    searchUserByName(na).display();
                    system("pause");
                    break;
                }
                system("pause");
                break;
            case 4:
                cout << "请输入需重置密码的用户名称：";
                cin >> na;
                if (searchUserByName(na).getName() != "NULL")
                {
                    searchUserByName(na).resetPassword();
                    File::wirteUsers(users);
                    system("cls");
                    cout << TITLE << endl;
                    cout << "重置成功！" << endl;
                    system("pause");
                    if (na == user.getName())
                    {
                        user.resetPassword();
                    }
                }
                break;
            case 5:
                cout << "请输入要更改用户名的用户名称：";
                cin >> na;
                if (searchUserByName(na).getName() != "NULL")
                {
                    cout << "请输入新用户名：";
                    cin >> pass;
                    searchUserByName(na).setName(pass);
                    File::wirteUsers(users);
                    system("cls");
                    cout << TITLE << endl;
                    cout << "修改成功！" << endl;
                    system("pause");
                    if (na == user.getName())
                    {
                        user.setName(pass);
                    }
                }
                else
                {
                    system("cls");
                    cout << TITLE << endl;
                    cout << "未查询到该用户" << endl;
                    system("pause");
                }
                break;
            case 6:
                cout << "请输入新密码：";
                cin >> pass;
                while (!isStandard(pass))
                {
                    cout << "密码不符合规范，请重新输入：";
                    cin >> pass;
                }
                searchUserByName(user.getName()).setPassword(pass);
                user.setPassword(pass);
                File::wirteUsers(users);
                system("cls");
                cout << TITLE << endl;
                cout << "修改成功,请重新登录！" << endl;
                out = ofstream("login.dat");
                out << "";
                out.close();
                system("pause");
                break;
            case 7:
                cout << "请输入要删除的账户名称：";
                cin >> na;
                if (na == user.getName())
                {
                    cout << "你不能删除自己哦~" << endl;
                    system("pause");
                    break;
                }
                if (searchUserByName(na).getName() != "NULL")
                {
                    users.erase(remove_if(users.begin(), users.end(), [na](const User &u)
                                          { return u.getName() == na; }),
                                users.end());
                    File::wirteUsers(users);
                    cout << "删除成功！" << endl;
                    system("pause");
                }
                else
                {
                    system("pause");
                }
                break;
            case 8:
                system("cls");
                cout << TITLE << endl;
                cout << "1. 借阅排行榜" << endl;
                cout << "2. 出版时间排行榜" << endl;
                cout << "请输入要查询的排行榜：";
                while (true)
                {
                    cin >> c;
                    if (c == 1 || c == 2)
                    {
                        break;
                    }
                    else
                    {
                        cout << "输入有误，请重新输入：";
                    }
                }
                switch (c)
                {
                case 1:
                    sortByRentTime();

                    break;
                case 2:
                    sortByPublishTime();
                    break;
                }
                break;
            case 9:
                out = ofstream("login.dat");
                out << "";
                out.close();
                cout << "已为您退出登录" << endl;
                system("pause");
                break;
            case 10:
                system("cls");
                cout << "已退出系统，欢迎再次使用！\n";
                system("pause");
                exit(0);
            default:
                cout << "无效的选择, 请重新输入：\n";
                system("pause");
            }
        } while (choice != 9 && choice != 10 && choice != 6);
    }
}