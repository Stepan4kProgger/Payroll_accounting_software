#include "Authentication.h"
using namespace std;

string Hasher(string pas, string salt) {
	int hash_buf = 1, p_size = pas.size();
	string hash = "";
	for (int i = 0; i < 4; i++)
		hash_buf += (int)salt[i] / p_size;
	hash_buf /= 25;
	for (int i = 0; i < p_size; i++)
		hash_buf += hash_buf / pas[i];
	for (int i = 0; i < 8; i++) {
		char add;
		if (i < 4)
			add = hash_buf + salt[i] / p_size;
		else
			add = hash_buf + salt[i - 4] % (hash_buf + p_size);
		if (add < 33) add += 38;
		hash += add;
	}
	return hash;
}

bool createFile() {
	int sw;
	system("cls");
	cout << "���� �� ����������. ������� ���?\n1 - ��\n";
	if (!(cin >> sw) || sw != 1) return false;
	ofstream f(users_file); f.close();
	cout << "���� ������. ������, ��� ������ � �� ��� �� ����� ������� ������\n"; //������� ������ ����� ������������, �������������� ��������� ��������
	system("pause");
	return true;
}

bool isFileEmpty() {
	ifstream file(users_file, ios::binary | ios::ate);
	if (file.tellg()) return false;
	cout << "���� ����!\n";
	return true;
}

bool isLoginInUse(string login) {
	ifstream file(users_file);
	string str;
	while (getline(file, str))
		if (strToUser(str).login == login)
			return true;
	return false;
}

string enterString(string title, bool is_password = false) {
	bool arrow = false;
	string str = empty_string;
	cout << title << ":\n";
	while (true) {
		char act = _getch();
		int ch = static_cast<int>(act);

		if (ch == 27) return empty_string;
		if (ch == 13) {
			if (str == empty_string) continue;
			cout << endl;
			return str;
		}

		if (arrow) {
			arrow = false;
			continue;
		}
		if (ch == -32) {
			arrow = true;
			continue;
		}

		if (ch == 8) {
			if (!str.length()) continue;
			str.resize(str.length() - 1);
			cout << backspace_attribute;
		}
		else {
			if (!isLoginLetter(act)) continue;
			if (str.length() >= max_string_length) {
				�learRow();
				cout << "������ ����� ���� �� ����� ��������� " << max_string_length << "! ������� ����� ������� ��� �����������...";
				str = _getch();
				�learRow();
				str = empty_string;
				continue;
			}
			str += act;
			if (is_password) cout << password_star;
			else cout << act;
		}
	}
	return str;
}

bool isLoggedIn(string log, string pas, int &role) {
	ifstream file(users_file);
	string str;
	bool answer = false;
	while (getline(file, str)) {
		User user = strToUser(str);
		if (user.login == log)
			if (user.password == Hasher(pas, user.salt)) {
				answer = true;
				if (!user.access) {
					cout << "������ ������� ������ �� ������������. ���������� � ��������������\n";
					return false;
				}
				else role = user.role;
				break;
			}
			else break;
	}
	if (!answer)
		cout << "����� ��� ������ �� ���������\n";
	return answer;
}

int LogIn(string &login) {
	string password;
	system("cls");
	if (isFileEmpty()) {
		system("pause");
		return 2;
	}
	cout << "��������� ���� ���� ��� ������������� ��������� (esc - ������)\n";
	login = enterString("�����");
	if (login == "") return 2;
	password = enterString("������", true);
	if (password == "") return 2;
	int role;
	if (!(isLoggedIn(login, password, role))) {
		system("pause");
		return 3;
	}
	return role;
}

void regNewUser(string login, string password) {
	User user;
	string salt = "";
	srand(time(NULL));
	for (int i = 0; i < 4; i++) {
		char inp = rand() % 126 + 33;
		salt += inp;
	}
	user.login = login;
	user.password = Hasher(password, salt);
	user.salt = salt;
	user.role = user.access = 0;
	addInfoToFile(users_file, userToStr(user));
}

int Register() {
	system("cls");
	string login, password, repeat_password;
	cout << "��������� ���� ���� ��� ����������� (esc - ������)\n";
	if (isLoginInUse(login = enterString("�����"))) {
		cout << "������ ����� �����. ����������� ������\n";
		system("pause");
		return 1;
	}
	if (login == "") return 2;
	password = enterString("������", true);
	if (password == "") return 2;
	repeat_password = enterString("��������� ������", true);
	if (repeat_password == "") return 2;
	if (password == repeat_password)
		regNewUser(login, password);
	else {
		cout << "������ �� ���������\n";
		system("pause");
		return 1;
	}
	cout << "����������� ���������. ��������� ������������� ������� ������ ���������������\n";
	system("pause");
	return 2;
}