#include "Menu.h"
using namespace std;

HANDLE hWndConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int ShowMenu(string title, ...) {
	
	va_list vl;  //получаем список переменных
	va_start(vl, title); // получаем указатель на title

	vector<string> menuItems; //вектор строк menu
	string menuItem = va_arg(vl, char*);
	while (menuItem != eof_menu) {
		menuItems.emplace_back(menuItem);
		menuItem = va_arg(vl, char*);
	}
	int menuItemsCount = (int)menuItems.size(); //получаем количество пунктов меню
	int pos = 1, prev = 0; //pos - текущий элемент меню, prev - предыдущий
	int menuIndex = -1;

	system("cls");
	cout << title << ":";

	SetConsoleTextAttribute(hWndConsole, 31);
	setCur(0, getConsoleSize().Y - 2);
	cout << "Используйте стрелки для навигации и нажмите Enter (или пробел), либо нажмите цифру на клавиатуре";

	while (menuIndex == -1) {
		if (pos != prev) { // не перерисовывать если ничего не изменилось
			for (int i = 1; i <= menuItemsCount; i++) {
				setCur(2, i + 1);
				if (pos == i) SetConsoleTextAttribute(hWndConsole, 240);
				else SetConsoleTextAttribute(hWndConsole, 15); //белый
				cout << i << ". " << menuItems[i - 1];
			}
			prev = pos;
		}

		setCur(0, getConsoleSize().Y - 1);  // опустить курсор
		SetConsoleTextAttribute(hWndConsole, 15);

		char key = _getch();
		int key_int = (int)key - 48;

		if (key_int > 0 && key_int <= menuItemsCount) return key_int;  // если нажата цифра
		switch (key)
		{
		case 72:  //вверх
			if (pos > 1) pos--;
			break;
		case 80:  //вниз
			if (pos < menuItemsCount) pos++;
			break;
		case 13: case 32: //ввод  пробел
			menuIndex = pos;
			break;
		case 27: //esc
			menuIndex = 0;
			break;
		default:
			break;
		}
	}
	system("cls");
	return menuIndex;
}