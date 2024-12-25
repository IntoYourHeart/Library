#ifndef BOOK
#define BOOK
#include <iostream>
using namespace std;
class Book {
	private:
		string name;
		string ISBN;
		string ISSN;
		string author;
		string publish;
		int num = 1;
		int rent_time = 0;
		int year;
		int month;
		double price;
		int rented_num=0;
		string introduction="No introduction";
	public:
		Book();
		Book(string name);
		Book(string name,string ISBN,string ISSN,string author\
		,string publish,double price,string introduction,int year,int month);
		Book(string name,string ISBN,string ISSN,string author\
		,string publish,double price,string introduction,int year,int month,int rentedNum);
		Book(string name,string ISBN,string ISSN,string author\
		,string publish,double price,string introduction,int year,int month,int rentedNum, int rent_time, int num);
		void setName(string name);
		string getName() const;
		void adminDisplay() const;
		void addRentTime();
		int getRentTime() const;
		void setISBN(string ISBN);
		string getISBN() const;
		int getYear() const;
		void setRentedNum(int rentedNum);
		int getMonth() const;
		void setNum(int num);
		int getNum() const;
		void addNum();
		void reduceNum();
		void setYear(int year);
		void setMonth(int month);
		void setIntroduction(string introduction);
		void addRentedNum();
		void reduceRentedNum();
		bool isRented() const;
		string getIntroduction() const;
		void setISSN(string ISSN);
		string getISSN() const;
		void setAuthor(string author);
		string getAuthor() const;
		void setPublish(string publish);
		string getPublish() const;
		void setPrice(double price);
		double getPrice() const;
		int getRentedNum() const;
		void display() const;
};
#endif