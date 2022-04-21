#include "UserFunctional.h"
using namespace std;

const char* DefineRoleToChange(int inp) {
	if (inp == 0) return mark_as_admin;
	return mark_as_user;
}

const char* DefineAccessToChange(int inp) {
	if (inp == 0) return mark_access_allow;
	return mark_access_deny;
}

string enterRow(string title, bool is_numeric = false) {
	bool arrow = false;
	bool dot_again = false;
	string str = "";
	cout << title << ":\n";
	while (true) {
		char ch = _getch();

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
		if (ch == -32 && _kbhit()) {
			arrow = true;
			continue;
		}

		if (ch == 8) {
			if (!str.length()) continue;
			if (str[str.length() - 1] == '.')
				dot_again = false;
			str.resize(str.length() - 1);
			cout << backspace_attribute;
		}
		else {
			if (is_numeric) {
				if (!isNumericLetter(ch)) continue;
				if (ch == '.' && dot_again) continue;
			}
			else if (!isNameLetter(ch)) continue;
			if ((str.length() >= max_name_length) || (is_numeric && str.length() >= max_salary_length)) {
				�learRow();
				cout << "������ ����� ���� �� ����� ��������� ";
				if (!is_numeric) cout << max_name_length;
				else {
					cout << max_salary_length;
					dot_again = false;
				}
				cout << "! ������� ����� ������� ��� �����������...";
				ch = _getch();
				�learRow();
				str = empty_string;
				continue;
			}
			if (ch == '.')
				dot_again = true;
			str += ch;
			cout << ch;
		}
	}
	return str;
}

Session::Session(string login, int role) {
	system("cls");
	this->login = login;
	this->role = role;
	formLists();
	cout << "�� ������� ����� � ������� ������\n";
	Sleep(1000);
}

Session::~Session() {
	system("cls");
	cout << "��������� ���������?\n1 - ��\n���� ���������� �� ���������, ������� ������ ������� (�������� esc)\n";
	int in = _getch();
	if (in == 1)
		SaveAll();
	system("cls");
	cout << "�� ������� ����� �� ������� ������\n";
	Sleep(1000);
}

void Session::formLists() {
	string str;

	ifstream file(users_file);
	while (getline(file, str))
		users.push_back(strToUser(str));
	user_amount = users.size();
	file.close();

	if (!isFileExist(database_file)) {
		ofstream f(database_file); f.close();
	}

	file.open(database_file);
	while (getline(file, str))
		workers.push_back(strToWorker(str));
	worker_amount = workers.size();
	file.close();
}

void Session::sortFunc(int option, bool rev) {
	int i, j, m;
	for (i = 0; i < worker_amount - 1; i++) {
		m = i;
		for (j = i + 1; j < worker_amount; j++)
			switch (option) {
			case 1:
				if (compare(buf[j].full_name, buf[m].full_name, rev)) m = j;
				break;
			case 2:
				if (compare(buf[j].department_name, buf[m].department_name, rev)) m = j;
				break;
			case 3:
				if (compare(buf[j].post, buf[m].post, rev)) m = j;
				break;
			case 4:
				if (compare(buf[j].salary_size, buf[m].salary_size, rev)) m = j;
				break;
			}
		Worker r = buf[m];
		buf[m] = buf[i];
		buf[i] = r;
	}
}


void Session::ShowWorkers(bool show_buf) {
	system("cls");
	if (!show_buf)
		if (isListEmpty(worker_amount))
			return;
	int limit;
	if (!show_buf) limit = worker_amount;
	else limit = buf.size();

	float colsWidthP[] = { 4.0f, .4f, .25f, .2f }; // ������ � ���������. ������ ������� � ��������, ��������� ������� �� �����������
	int colCount = sizeof(colsWidthP) / sizeof(float);
	drawTaleText(colsWidthP, colCount, true, headerWorkerText);
	for (int i = 0; i < limit; i++) {
		stringstream ss;
		if (!show_buf) {
			ss << fixed << setprecision(2) << workers[i].salary_size;
			string Text[] = {to_string(i + 1), workers[i].full_name, workers[i].department_name, workers[i].post, ss.str()};
			drawTaleText(colsWidthP, colCount, false, Text);
		}
		else {
			ss << fixed << setprecision(2) << buf[i].salary_size;
			string Text[] = { to_string(i + 1), buf[i].full_name, buf[i].department_name, buf[i].post, ss.str() };
			drawTaleText(colsWidthP, colCount, false, Text);
		}
	}
	drawTableBorder(colsWidthP, colCount, 2);
}

void Session::IndividualTask(int option) {
	system("cls");
	if (isListEmpty(worker_amount))
		return;

	if (option == 1 || option == 2) {
		buf = workers;
		sortFunc(2);
		string str_buf = buf[0].department_name;
		int counter = 1, local_counter = 1;
		double sum = buf[0].salary_size;
		float colsWidthP[] = { 4.f, .5f }; // ������ � ���������. ������ ������� � ��������, ��������� ������� �� �����������
		int colCount = sizeof(colsWidthP) / sizeof(float);
		if (option == 1) drawTaleText(colsWidthP, colCount, true, headerTaskText);
		else drawTaleText(colsWidthP, colCount, true, headerTask2Text);
		for (int i = 1; i < worker_amount; i++) {
			if (str_buf != buf[i].department_name) {
				if (option == 2) sum /= local_counter;
				stringstream ss;
				ss << fixed << setprecision(2) << sum;
				string Text[] = { to_string(counter++), str_buf, ss.str() };

				drawTaleText(colsWidthP, colCount, false, Text);
				sum = buf[i].salary_size;
				str_buf = buf[i].department_name;
				if (option == 2) local_counter = 1;
			}
			else {
				sum += buf[i].salary_size;
				if (option == 2) local_counter++;
			}
		}
		if (option == 2) sum /= local_counter;
		stringstream ss;
		ss << fixed << setprecision(2) << sum;
		string Text[] = { to_string(counter++), str_buf, ss.str() };

		drawTaleText(colsWidthP, colCount, false, Text);;
		drawTableBorder(colsWidthP, colCount, 2);
	}
	else {
		double key;
		int counter = 0;
		string salary = enterRow("������� �������������� ������ �/� ���������� (��� ������� �������� ����������� \".\")", true);
		if (salary == empty_string) return;
		key = stod(salary);

		for (int i = 0; i < worker_amount; i++)
			if (workers[i].salary_size < key) {
				buf.push_back(workers[i]);
				counter++;
			}
		if (counter == 0) {
			cout << "����� ����������� �� �������\n";
			return;
		}
		ShowWorkers(true);
		cout << "������� " << counter << " ����������\n";
	}
	buf.clear();
	system("pause");
}

void Session::SearchWorkers() {
	system("cls");
	if (isListEmpty(worker_amount))
		return;
	while (true) {
		int answer = ShowMenu("�������� �������� ������ (esc - ������)",
			"�� ���",
			"�� ������",
			"�� ���������", eof_menu);
		string key;
		int counter = 0;
		
		system("cls");
		switch (answer) {
		case 1:
			key = enterRow("������� ��� ���������� (����� ������ ��������)");
			if (key == empty_string) break;
			for (int i = 0; i < worker_amount; i++)
				if (workers[i].full_name.find(key) != -1) {
					buf.push_back(workers[i]);
					counter++;
				}
			break;
		case 2:
			key = enterRow("������� ����� ���������� (����� ������ ��������)");
			if (key == empty_string) break;
			for (int i = 0; i < worker_amount; i++) 
				if (workers[i].department_name.find(key) != -1) {
					buf.push_back(workers[i]);
					counter++;
				}
			break;
		case 3:
			key = enterRow("������� ��������� ���������� (����� ������ ��������)");
			if (key == empty_string) break;
			for (int i = 0; i < worker_amount; i++) 
				if (workers[i].post.find(key) != -1) {
					buf.push_back(workers[i]);
					counter++;
				}
			break;
		case 0:
			break;
		}
		if (key == empty_string) break;
		else if (answer == 0) break;
		else if (counter != 0) {
			ShowWorkers(true);
			cout << "������� " << counter << " ����������\n";
		}
		else cout << "���������� �� �������\n";
		buf.clear();
		system("pause");
	}
}

void Session::SortWorkers() {
	system("cls");
	if (isListEmpty(worker_amount))
		return;
	bool reverse = false;
	cout << "�������� ������� ����������\n1 - �� �����������\n���� ����� - �� ��������\n";
	if (_getch() == '1') reverse = true;
	buf = workers;
	int answer = ShowMenu("�������� �������� ���������� (esc - ������)",
		"�� ���",
		"�� �������� ������",
		"�� �������� ���������",
		"�� ������� �/�", eof_menu);
	if (answer != 0) sortFunc(answer, reverse);
	else return;
	workers = buf;
	ShowWorkers();
	system("pause");
}

void Session::AdminFunctional() {
	while (true) {
		int answer = ShowMenu("�����-���� | �������� �������� (esc - ������� � �������� ����)",
			"�������� ����������",
			"�������� ������ ����������",
			"�������� ��� ������� ������",
			"�������� ������ ������� ������",
			"��������� ��� ��������� � ������", eof_menu);
		switch (answer) {
		case 1:
			AddWorker();
			break;
		case 2:
			EditWorker();
			break;
		case 3:
			ShowUsers();
			system("pause");
			break;
		case 4:
			EditUser();
			break;
		case 5:
			SaveAll();
			break;
		case 0:
			return;
		}
	}
}

void Session::AddWorker() {
	system("cls");
	cout << "�� ����� ����� ����� �� ������ ��������� � ����, ����� esc\n";
	Worker worker;
	worker.full_name = enterRow("������� ��� ����������");
	if (worker.full_name == empty_string) return;
	worker.department_name = enterRow("������� ����� ����������");
	if (worker.department_name == empty_string) return;
	worker.post = enterRow("������� ��������� ����������");
	if (worker.post == empty_string) return;
	string salary = enterRow("������� �������������� ������ �/� ���������� (��� ������� �������� ����������� \".\")", true);
	if (salary == empty_string) return;
	worker.salary_size = stod(salary);
	workers.push_back(worker);
	worker_amount++;
	cout << "��������� ������� ��������\n";
	system("pause");
}

void Session::EditWorker() {
	while (true) {
		system("cls");
		if (isListEmpty(worker_amount))
			return;
		ShowWorkers();
		int num;
		cout << "������� ����� ����������, ������ �������� �� ������ �������� (0 - ������)\n";
		if (!(cin >> num) || num < 1 || num > worker_amount) {
			system("cls");
			cout << "������ �� ��������\n";
			system("pause");
			return;
		}
		Worker temp = workers[--num];
		string str;
		while (true) {
			int answer = ShowMenu("��������� ���������: \"" + workers[num].full_name + "\" | �������� �������� (esc - ������)",
				"�������� ���",
				"�������� �����",
				"�������� ���������",
				"�������� ������ �/�",
				"������� ����������", eof_menu);
			system("cls");
			switch (answer) {
			case 1:
				str = enterRow("������� ��� ����������");
				if (str == empty_string) break;
				temp.full_name = str;
				break;
			case 2:
				str = enterRow("������� ����� ����������");
				if (str == empty_string) break;
				temp.department_name = str;
				break;
			case 3:
				str = enterRow("������� ��������� ����������");
				if (str == empty_string) break;
				temp.post = str;
				break;
			case 4:
				str = enterRow("������� �������������� ������ �/� ���������� (��� ������� �������� ����������� \".\")", true);
				if (str == empty_string) break;
				temp.salary_size = stod(str);
				break;
			case 5:
				workers.erase(workers.begin() + num);
				worker_amount--;
				break;
			case 0:
				break;
			}
			if (answer == 0) break;
			if (answer != 5) {
				workers[num] = temp;
				cout << "������ �������� �������\n";
				system("pause");
			}
			else {
				cout << "��������� ������� ����� �� ������\n";
				system("pause");
				break;
			}
		}
	}
}

void Session::ShowUsers() {

	system("cls");
	if (isListEmpty(user_amount))
		return;

	float colsWidthP[] = { 4.f, .3f , .3f }; // ������ � ���������. ������ ������� � ��������, ��������� ������� �� �����������
	int colCount = sizeof(colsWidthP) / sizeof(float);
	drawTaleText(colsWidthP, colCount, true, headerUserText);
	for (int i = 0; i < user_amount; i++) {
		string role, access;
		if (users[i].role) role = "�������������";
		else role = "������������";
		if (users[i].access) access = "����";
		else access = "�� ����������";
		string Text[] = { to_string(i + 1), users[i].login, role, access };

		drawTaleText(colsWidthP, colCount, false, Text);
	}

	drawTableBorder(colsWidthP, colCount, 2);
}

void Session::EditUser() {
	while (true) {
		system("cls");
		if (isListEmpty(user_amount))
			return;
		ShowUsers();
		int num;
		cout << "������� ����� ������� ������, ������ ������� �� ������ �������� (0 - ������)\n";
		if (!(cin >> num) || num < 1 || num > user_amount) {
			system("cls");
			cout << "������ �� ��������\n";
			system("pause");
			return;
		}
		User temp = users[--num];
		if (temp.login == login) {
			system("cls");
			cout << "� ����� ������������, �� �� ������ ������������� ������� ������� ������\n";
			system("pause");
			continue;
		}
		
		while (true) {
			int answer = ShowMenu("������� ������� ������ � �������: \"" + users[num].login + "\" | �������� �������� (esc - ������)",
				DefineRoleToChange(temp.role),
				DefineAccessToChange(temp.access),
				"������� ������� ������", eof_menu);
			system("cls");
			switch (answer) {
			case 1:
				if (temp.role == 0)
					temp.role = 1;
				else
					temp.role = 0;
				break;
			case 2:
				if (temp.access == 0) 
					temp.access = 1;
				else 
					temp.access = 0;
				break;
			case 3:
				users.erase(users.begin() + num);
				user_amount--;
				break;
			case 0:
				break;
			}
			if (answer == 0) break;
			if (answer != 3) {
				users[num] = temp;
				cout << "������ �������� �������\n";
				system("pause");
			}
			else {
				cout << "������� ������ ������� ������� �� ������\n";
				system("pause");
				break;
			}
		}
	}
}

void Session::SaveAll() {
	ofstream file(users_file); //������� � ������
	file.close();
	file.open(users_file, ios_base::app);
	for (int i = 0; i < user_amount; i++)
		file << userToStr(users[i]) + "\n";
	file.close();
	file.open(database_file);
	file.close();
	file.open(database_file, ios_base::app);
	for (int i = 0; i < worker_amount; i++)
		file << workerToStr(workers[i]) + "\n";
	file.close();
	system("cls");
	cout << "��� ��������� ��������� �������\n";
	system("pause");
}

void initializeUser(string login, int role) {
	Session user(login, role);
	while (true) {
		int answer;
		if (role == 0)
			answer = ShowMenu("����� ����������, ������������! | �������� �������� (esc - ����� �� ������� ������)",
				"�������� ������� ������ �����������",
				"��������� ����� ����� ������ �� ����� �� ������� ������",
				"��������� �������������� ��������� ����������� �� ������� ������",
				"������� ������ �����������, � ������� �������� ���� �������� � ����������",
				"����� �� ������",
				"���������� �� ������", eof_menu);
		else
			answer = ShowMenu("����� ����������, �������������! | �������� �������� (esc - ����� �� ������� ������)",
				"�������� ������� ������ �����������",
				"��������� ����� ����� ������ �� ����� �� ������� ������",
				"��������� �������������� ��������� ����������� �� ������� ������",
				"������� ������ �����������, � ������� �������� ���� �������� � ����������",
				"����� �� ������",
				"���������� �� ������",
				"������� � �������������� ������������ ��������������", eof_menu);
		switch (answer) {
		case 1:
			user.ShowWorkers();
			system("pause");
			break;
		case 2: case 3: case 4:
			user.IndividualTask(answer - 1);
			break;
		case 5:
			user.SearchWorkers();
			break;
		case 6:
			user.SortWorkers();
			break;
		case 7:
			user.AdminFunctional();
			break;
		case 0:
			return;
		}
	}
}