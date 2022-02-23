//
// Created by Alejandro D on 22/02/22.
//

#include <iostream>

using namespace std;

void clear() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

string quitar(string &txt, char patron) {
  string result = string("");
  for (int i = 0; i < txt.length(); i++) {
    if (patron != txt[i])
      result += txt[i];
  }

  return result;
}
