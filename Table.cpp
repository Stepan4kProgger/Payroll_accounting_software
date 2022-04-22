#include "Table.h"
using namespace std;

void coutSymbol(char symb, int count) {
    for (; count >= 0; count--) cout << symb;
}

string stringSerialize(string text, int width) {
    if (text.length() > width) {
        text.resize(width - 3);
        return text + "...";
    }
    for (int i = text.length(); i < width; i++) text += " ";
    return text;
}

void drawTableBorder(float* colsWidthP, int colCount, byte type) {
    UINT currentConsoleCP = GetConsoleOutputCP();
    SetConsoleOutputCP(866);

    char borderSymbs[3][4] = { {(char)196 /* ─ */, (char)218 /* ┌ */, (char)194 /* ┬ */, (char)191 /* ┐ */ }, // type = 0 верхушка
                               {(char)205 /* ═ */, (char)198 /* ╞ */, (char)216 /* ╪ */, (char)181 /* ╡ */},  // type = 1 шапка таблицы
                               {(char)196 /* ─ */, (char)192 /* └ */, (char)193 /* ┴ */, (char)217 /* ┘ */} };// type = 1 низ

    int numberWidth = (int)colsWidthP[0];
    int consWidth = (int)getConsoleSize().X - numberWidth + 1; // numberWidth символа для N

    cout << borderSymbs[type][1];
    coutSymbol(borderSymbs[type][0], numberWidth);  //для N
    int cursorPosition = numberWidth + 1;

    for (int i = 1; i < colCount; i++) {
        int colsWidthS = consWidth * colsWidthP[i];
        cout << borderSymbs[type][2];
        coutSymbol(borderSymbs[type][0], colsWidthS - 1);
        cursorPosition += colsWidthS + 1;
    }

    cout << borderSymbs[type][2];
    coutSymbol(borderSymbs[type][0], (int)getConsoleSize().X - cursorPosition - 4);
    cout << borderSymbs[type][3];

    SetConsoleOutputCP(currentConsoleCP);
}

void drawTaleText(float* colsWidthP, int colCount, bool header, string* text) {
    if (header) drawTableBorder(colsWidthP, colCount, 0);

    UINT currentConsoleCP = GetConsoleOutputCP();
    SetConsoleOutputCP(866);

    int numberWidth = (int)colsWidthP[0];
    int consWidth = (int)getConsoleSize().X - numberWidth + 1; // numberWidth символа для N
    char razd = (char)179; //179 - │

    int cursorPosition = 1;

    for (int i = 0; i < colCount; i++) {
        int colsWidthS = consWidth * colsWidthP[i];
        if (i == 0) colsWidthS = numberWidth + 1;
        SetConsoleOutputCP(866);
        cout << razd;
        SetConsoleOutputCP(1251);

        cout << stringSerialize(text[i], colsWidthS);

        cursorPosition += colsWidthS + 1;
    }

    SetConsoleOutputCP(866);
    cout << razd;
    SetConsoleOutputCP(1251);

    int lastColWidth = (int)getConsoleSize().X - cursorPosition - 1;
    cout << stringSerialize(text[colCount], lastColWidth);

    SetConsoleOutputCP(866);
    cout << razd;

    if (header) drawTableBorder(colsWidthP, colCount, 1);
    SetConsoleOutputCP(currentConsoleCP);
}