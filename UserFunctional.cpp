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
				сlearRow();
				cout << "Размер этого поля не может превышать ";
				if (!is_numeric) cout << max_name_length;
				else {
					cout << max_salary_length;
					dot_again = false;
				}
				cout << "! Нажмите любую клавишу для продолжения...";
				ch = _getch();
				сlearRow();
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
	cout << "Вы успешно вошли в учётную запись\n";
	Sleep(1000);
}

Session::~Session() {
	system("cls");
	cout << "Сохранить изменения?\n1 - Да\nЕсли сохранение не требуется, нажмите другую клавишу (например esc)\n";
	int in = _getch();
	if (in == 1)
		SaveAll();
	system("cls");
	cout << "Вы успешно вышли из учётной записи\n";
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

	float colsWidthP[] = { 4.0f, .4f, .25f, .2f }; // размер в процентах. первый столбец в символах, последний столбец не указывается
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
		float colsWidthP[] = { 4.f, .5f }; // размер в процентах. первый столбец в символах, последний столбец не указывается
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
		string salary = enterRow("Введите среднемесячный размер з/п сотрудника (для дробной величины используйте \".\")", true);
		if (salary == empty_string) return;
		key = stod(salary);

		for (int i = 0; i < worker_amount; i++)
			if (workers[i].salary_size < key) {
				buf.push_back(workers[i]);
				counter++;
			}
		if (counter == 0) {
			cout << "Таких сотрудников не найдено\n";
			return;
		}
		ShowWorkers(true);
		cout << "Найдено " << counter << " совпадений\n";
	}
	buf.clear();
	system("pause");
}

void Session::SearchWorkers() {
	system("cls");
	if (isListEmpty(worker_amount))
		return;
	while (true) {
		int answer = ShowMenu("Выберите параметр поиска (esc - отмена)",
			"По ФИО",
			"По отделу",
			"По должности", eof_menu);
		string key;
		int counter = 0;
		
		system("cls");
		switch (answer) {
		case 1:
			key = enterRow("Введите ФИО сотрудника (можно ввести частично)");
			if (key == empty_string) break;
			for (int i = 0; i < worker_amount; i++)
				if (workers[i].full_name.find(key) != -1) {
					buf.push_back(workers[i]);
					counter++;
				}
			break;
		case 2:
			key = enterRow("Введите отдел сотрудника (можно ввести частично)");
			if (key == empty_string) break;
			for (int i = 0; i < worker_amount; i++) 
				if (workers[i].department_name.find(key) != -1) {
					buf.push_back(workers[i]);
					counter++;
				}
			break;
		case 3:
			key = enterRow("Введите должность сотрудника (можно ввести частично)");
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
			cout << "Найдено " << counter << " совпадений\n";
		}
		else cout << "Совпадений не найдено\n";
		buf.clear();
		system("pause");
	}
}

void Session::SortWorkers() {
	system("cls");
	if (isListEmpty(worker_amount))
		return;
	bool reverse = false;
	cout << "Выберите порядок сортировки\n1 - По возрастанию\nИной выбор - по убыванию\n";
	if (_getch() == '1') reverse = true;
	buf = workers;
	int answer = ShowMenu("Выберите параметр сортировки (esc - отмена)",
		"По ФИО",
		"По названию отдела",
		"По названию должности",
		"По размеру з/п", eof_menu);
	if (answer != 0) sortFunc(answer, reverse);
	else return;
	workers = buf;
	ShowWorkers();
	system("pause");
}

void Session::AdminFunctional() {
	while (true) {
		int answer = ShowMenu("Админ-меню | Выберите действие (esc - перейти в основное меню)",
			"Добавить сотрудника",
			"Изменить данные сотрудника",
			"Просмотр все учётные записи",
			"Изменить данные учётной записи",
			"Применить все изменения к файлам", eof_menu);
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
	cout << "На любом этапе ввода Вы можете вернуться в меню, нажав esc\n";
	Worker worker;
	worker.full_name = enterRow("Введите ФИО сотрудника");
	if (worker.full_name == empty_string) return;
	worker.department_name = enterRow("Введите отдел сотрудника");
	if (worker.department_name == empty_string) return;
	worker.post = enterRow("Введите должность сотрудника");
	if (worker.post == empty_string) return;
	string salary = enterRow("Введите среднемесячный размер з/п сотрудника (для дробной величины используйте \".\")", true);
	if (salary == empty_string) return;
	worker.salary_size = stod(salary);
	workers.push_back(worker);
	worker_amount++;
	cout << "Сотрудник успешно добавлен\n";
	system("pause");
}

void Session::EditWorker() {
	while (true) {
		system("cls");
		if (isListEmpty(worker_amount))
			return;
		ShowWorkers();
		int num;
		cout << "Введите номер сотрудника, данные которого вы хотите изменить (0 - отмена)\n";
		if (!(cin >> num) || num < 1 || num > worker_amount) {
			system("cls");
			cout << "Ничего не изменено\n";
			system("pause");
			return;
		}
		Worker temp = workers[--num];
		string str;
		while (true) {
			int answer = ShowMenu("Выбранный сотрудник: \"" + workers[num].full_name + "\" | Выберите действие (esc - отмена)",
				"Изменить ФИО",
				"Изменить отдел",
				"Изменить должность",
				"Изменить размер з/п",
				"Удалить сотрудника", eof_menu);
			system("cls");
			switch (answer) {
			case 1:
				str = enterRow("Введите ФИО сотрудника");
				if (str == empty_string) break;
				temp.full_name = str;
				break;
			case 2:
				str = enterRow("Введите отдел сотрудника");
				if (str == empty_string) break;
				temp.department_name = str;
				break;
			case 3:
				str = enterRow("Введите должность сотрудника");
				if (str == empty_string) break;
				temp.post = str;
				break;
			case 4:
				str = enterRow("Введите среднемесячный размер з/п сотрудника (для дробной величины используйте \".\")", true);
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
				cout << "Данные изменены успешно\n";
				system("pause");
			}
			else {
				cout << "Сотрудник успешно удалён из списка\n";
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

	float colsWidthP[] = { 4.f, .3f , .3f }; // размер в процентах. первый столбец в символах, последний столбец не указывается
	int colCount = sizeof(colsWidthP) / sizeof(float);
	drawTaleText(colsWidthP, colCount, true, headerUserText);
	for (int i = 0; i < user_amount; i++) {
		string role, access;
		if (users[i].role) role = "Администратор";
		else role = "Пользователь";
		if (users[i].access) access = "Есть";
		else access = "Не подтверждён";
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
		cout << "Введите номер учётной записи, данные которой вы хотите изменить (0 - отмена)\n";
		if (!(cin >> num) || num < 1 || num > user_amount) {
			system("cls");
			cout << "Ничего не изменено\n";
			system("pause");
			return;
		}
		User temp = users[--num];
		if (temp.login == login) {
			system("cls");
			cout << "В целях безопасности, вы не можете редактировать текущую учётную запись\n";
			system("pause");
			continue;
		}
		
		while (true) {
			int answer = ShowMenu("Выбрана учётная запись с логином: \"" + users[num].login + "\" | Выберите действие (esc - отмена)",
				DefineRoleToChange(temp.role),
				DefineAccessToChange(temp.access),
				"Удалить учётную запись", eof_menu);
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
				cout << "Данные изменены успешно\n";
				system("pause");
			}
			else {
				cout << "Учётная запись успешно удалена из списка\n";
				system("pause");
				break;
			}
		}
	}
}

void Session::SaveAll() {
	ofstream file(users_file); //очистка и запись
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
	cout << "Все изменения сохранены успешно\n";
	system("pause");
}

void initializeUser(string login, int role) {
	Session user(login, role);
	while (true) {
		int answer;
		if (role == 0)
			answer = ShowMenu("Добро пожаловать, пользователь! | Выберите действие (esc - выйти из учётной записи)",
				"Просмотр полного списка сотрудников",
				"Вычислить общую сумму выплат за месяц по каждому отделу",
				"Вычислить среднемесячный заработок сотрудников по каждому отделу",
				"Вывести список сотрудников, у которых зарплата ниже введённой с клавиатуры",
				"Поиск по данным",
				"Сортировка по данным", eof_menu);
		else
			answer = ShowMenu("Добро пожаловать, администратор! | Выберите действие (esc - выйти из учётной записи)",
				"Просмотр полного списка сотрудников",
				"Вычислить общую сумму выплат за месяц по каждому отделу",
				"Вычислить среднемесячный заработок сотрудников по каждому отделу",
				"Вывести список сотрудников, у которых зарплата ниже введённой с клавиатуры",
				"Поиск по данным",
				"Сортировка по данным",
				"Перейти к функциональным возможностям администратора", eof_menu);
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