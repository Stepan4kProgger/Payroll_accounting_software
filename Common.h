#pragma once
#include <Windows.h>
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>
using std::string;

class User {
public:
	string login, password, salt;
	int role, access; //role: 0 - user, 1 - admin ||| access: 1 - allow, 0 - deny;
};

class Worker {
public:
	string full_name, department_name, post;
	double salary_size;
};

const string empty_string = "";
const string backspace_attribute = "\b \b";
const string users_file = "UserData.txt";
const string database_file = "Database.txt";
const char password_star = '*';
const unsigned int max_string_length = 32;
const unsigned int max_name_length = 128;
const unsigned int max_salary_length = 10;

COORD getConsoleSize();
void ñlearRow();
void addInfoToFile(string, string);
bool isFileExist(const string);
User strToUser(string);
string userToStr(User);
Worker strToWorker(string);
string workerToStr(Worker);