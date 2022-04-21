#pragma once
#include "Menu.h"
#include "Table.h"
#include "Algorithms.h"
using std::string;
using std::vector;

void initializeUser(string, int);

const char mark_as_admin[26] = "Назначить администратором";
const char mark_as_user[24] = "Назначить пользователем";
const char mark_access_allow[19] = "Подтвердить доступ";
const char mark_access_deny[21] = "Заблокировать доступ";

class Session {
	string login;
	string headerUserText[4] = { "№","Логин","Уровень доступа","Доступ" };
	string headerWorkerText[5] = { "№","ФИО сотрудника","Отдел","Должность","Размер з/п" };
	string headerTaskText[3] = { "№", "Отдел", "Общая сумма выплат" };
	string headerTask2Text[3] = { "№", "Отдел", "Среднемесячный заработок сотрудников" };
	vector<Worker> buf;
	vector<Worker> workers;
	vector<User> users;
	unsigned int worker_amount = 0, user_amount = 0;
	int role;
public:
	Session(string, int);
	~Session();
	void formLists();
	void ShowWorkers(bool = false);
	void sortFunc(int, bool = false);
	void IndividualTask(int);
	void SearchWorkers();
	void SortWorkers();
	void AdminFunctional();
	void AddWorker();
	void EditWorker();
	void ShowUsers();
	void EditUser();
	void SaveAll();
};