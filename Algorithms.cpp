#include "Algorithms.h"
using namespace std;

bool compare(string left, string right, bool reverse) {
	int minLen = min(left.size(), right.size());
	for (int i = 0; i < minLen; i++) {
		if (left[i] < right[i])
			if (reverse) return false;
			else return true;
		if (left[i] > right[i])
			if (reverse) return true;
			else return false;
	}
	return false;
}

bool compare(double left, double right, bool reverse) {
	if (left < right)
		if (reverse) return true;
		else return false;
	if (left > right)
		if (reverse) return false;
		else return true;
	return false;
}

bool isNameLetter(char inp) {
	if ((inp >= 'А' && inp <= 'Я') ||
		(inp >= 'а' && inp <= 'я') ||
		inp == '-' || inp == ' ' || inp == '\'') return true;
	return false;
}

bool isNumericLetter(char inp) {
	if ((inp >= '0' && inp <= '9') || inp == '.') return true;
	return false;
}

bool isLoginLetter(char inp) {
	if ((inp >= '0' && inp <= '9') ||
		(inp >= 'A' && inp <= 'Z') ||
		(inp >= 'a' && inp <= 'z') ||
		inp == '_' || inp == '-') return true;
	return false;
}

bool isListEmpty(int list_size) {
	if (list_size != 0) return false;
	system("cls");
	cout << "Список пуст!\n";
	system("pause");
	return true;
}