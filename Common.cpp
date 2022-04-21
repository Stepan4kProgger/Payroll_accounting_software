#include "Common.h"
using namespace std;

void setCur(SHORT x, SHORT y) {
	COORD cursor = { x,y };
	HANDLE hWndConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hWndConsole, cursor);
}

COORD getConsoleSize() {
	COORD size = { 0,0 };
	HANDLE hWndConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo)) {
		size.X = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
		size.Y = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
	}
	return size;
}

void сlearRow() {
	cout << "\r";
	for (int i = 0; i < 16; i++) cout << "      ";
	cout << "\r";
}

bool isFileExist(string filename) {
	ifstream file(filename);
	return file.is_open(); //закроется самостоятельно
}

void addInfoToFile(string filename, string info) {
	ofstream file(filename, ios_base::app);
	file << info << '\n';
	file.close();
}

Worker strToWorker(string str) {
	stringstream strS(str);
	Worker temp;

	string substr;
	getline(strS, substr, '\t');
	temp.full_name = substr;
	getline(strS, substr, '\t');
	temp.department_name = substr;
	getline(strS, substr, '\t');
	temp.post = substr;
	getline(strS, substr, '\t');
	temp.salary_size = stod(substr);

	return temp;
}

string workerToStr(Worker work) {
	stringstream ss;
	ss << fixed << setprecision(2) << work.salary_size;
	return work.full_name + '\t' + work.department_name + '\t' + work.post + '\t' + ss.str();
}

User strToUser(string str) {
	stringstream strS(str);
	User temp;

	string substr;
	getline(strS, substr, '\t');
	temp.login = substr;
	getline(strS, substr, '\t');
	temp.password = substr;
	getline(strS, substr, '\t');
	temp.salt = substr;
	getline(strS, substr, '\t');
	temp.role = stoi(substr);
	getline(strS, substr, '\t');
	temp.access = stoi(substr);
	return temp;
}

string userToStr(User user) {
	stringstream role, access;
	role << user.role;
	access << user.access;
	return user.login + '\t' + user.password + '\t' + user.salt + '\t' + role.str() + '\t' + access.str();
}