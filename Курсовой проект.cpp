#include "UserFunctional.h"
#include "Authentication.h"
using namespace std;

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string login;
	while (true) {
		int answer, menu = ShowMenu("Программа учёта выплат ЗП сотрудникам предприятия | Выберите действие (esc - выйти)",
			"Войти в учётную запись",
			"Зарегистрироваться", eof_menu);
		if (!isFileExist(users_file)) {
			if (createFile()) continue;
			else return -1;
		}
		if (menu == 1) {
			while (true) {
				answer = LogIn(login);
				if (answer == 3) continue;
				else if (answer == 2) break;
				else {
					initializeUser(login, answer);
					break;
				}
			}
		}
		if (menu == 2) {
			while (true) {
				answer = Register();
				if (answer == 1) continue;
				else break;
			}
		}
		if (menu == 0) return 0;
	}
	return 0;
}