//
// Created by Alejandro D on 22/02/22.
//

#include <iostream>

void clear() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}
