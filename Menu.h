#pragma once
#include "Common.h"
using std::string;

const char eof_menu[8] = "##End##";

void setCur(SHORT, SHORT);
int ShowMenu(string, ...);