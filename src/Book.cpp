#include <iostream>
#include "Book.h"
using namespace std;

Book::Book() {}
Book::Book(string name):name(name){}
Book::Book(string name, string ISBN, string ISSN, string author, string publish, double price, string introduction, int year, int month)
	: name(name), ISBN(ISBN), ISSN(ISSN), author(author), publish(publish), price(price), introduction(introduction), year(year), month(month) {}
Book::Book(string name, string ISBN, string ISSN, string author, string publish, double price, string introduction, int year, int month, int rentedNum)
	: name(name), ISBN(ISBN), ISSN(ISSN), author(author), publish(publish), price(price), introduction(introduction), year(year), month(month), rented_num(rentedNum) {}
Book::Book(string name, string ISBN, string ISSN, string author, string publish, double price, string introduction, int year, int month, int rentedNum, int rent_time, int num)
	: name(name), ISBN(ISBN), ISSN(ISSN), author(author), publish(publish), price(price), introduction(introduction), year(year), month(month), rented_num(rentedNum), rent_time(rent_time), num(num) {}
void Book::setName(string name)
{
	this->name = name;
}
string Book::getName() const
{
	return name;
}
string Book::getIntroduction() const
{
	return introduction;
}
void Book::setISBN(string ISBN)
{
	this->ISBN = ISBN;
}
string Book::getISBN() const
{
	return ISBN;
}
void Book::setISSN(string ISSN)
{
	this->ISSN = ISSN;
}
string Book::getISSN() const
{
	return ISSN;
}
void Book::setAuthor(string author)
{
	this->author = author;
}
string Book::getAuthor() const
{
	return author;
}
void Book::setPublish(string publish)
{
	this->publish = publish;
}
string Book::getPublish() const
{
	return publish;
}
void Book::setPrice(double price)
{
	this->price = price;
}
double Book::getPrice() const
{
	return price;
}
int Book::getNum() const{
	return num;
}
void Book::setNum(int num){
	this->num = num;
}
void Book::addNum(){
	this->num++;
}
bool Book::isRented() const{
	return num == rented_num;
}
void Book::reduceNum(){
	this->num--;
}
void Book::addRentedNum(){
	this->rented_num++;
}
void Book::reduceRentedNum(){
	this->rented_num--;
}
int Book::getRentedNum() const
{
	return rented_num;
}
void Book::setYear(int y){
	this->year = y;
}
void Book::setIntroduction(string introduction){
	this->introduction = introduction;
}
void Book::setMonth(int m){
	this->month = m;
}
int Book::getRentTime() const{
	return rent_time;
}
void Book::addRentTime(){
	this->rent_time++;
}
void Book::setRentedNum(int rentedNum)
{
	this->rented_num = rentedNum;
}
int Book::getYear() const
{
	return this->year;
}
int Book::getMonth() const
{
	return this->month;
}
void Book::display() const
{
	cout << "**********图书信息**********" << endl;
	cout << "图书名称:" << name << endl;
	cout << "作者:" << author << endl;
	cout << "出版社:" << publish << endl;
	cout << "出版时间:" << year << "年" << month << "月" << endl;
	cout << "借阅状态:" << (num == rented_num ? "不可借阅" : "可借阅") << endl;
	cout << "累计借阅次数：" << rent_time << endl;
	cout << "定价:" << price << endl;
	if (introduction != "No introduction" && introduction != "")
		cout << "简介:" << introduction << endl;
	cout << "****************************" << endl;
}

void Book::adminDisplay() const
{
	cout << "**********图书信息**********" << endl;
	cout << "图书名称：" << name << endl;
	cout << "作者：" << author << endl;
	cout << "出版社：" << publish << endl;
	cout << "出版时间：" << year << "年" << month << "月" << endl;
	cout << "库存：" << num << "本" << endl;
	if(rented_num == num){
		cout << "借阅状态：" << "不可借阅" << endl;
	}else{
		cout << "借阅状态：" << "可借阅数量：" << num-rented_num << endl;
	}
	cout << "累计借阅次数：" << rent_time << endl;
	cout << "定价:" << price << endl;
	if (introduction != "No introduction" && introduction != "")
		cout << "简介:" << introduction << endl;
	cout << "****************************" << endl;
}