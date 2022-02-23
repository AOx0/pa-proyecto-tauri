#ifndef ADMIN_UTILS_H
#define ADMIN_UTILS_H

#include <iostream>
#include <sstream>

using namespace std;

// Con ayuda de https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp/
template<typename V>
string pedirValor(V dep, const string &msg, bool (*func)(string &, V), const string &on_error) {
  string inp;
  bool valid = false;

  do {
    cout << msg;

    // de acuerdo a https://programmerclick.com/article/626810031/
    cin.clear();
    cin.sync();
    getline(cin, inp, '\n');

    if (!func(inp, dep))
      cout << on_error << endl;
    else valid = true;

  } while (!valid);

  return inp;
}

template<typename V>
string pedirValor(V dep, const string &msg, bool (*func)(string &, V), const string &on_error, int intentos) {
  string inp;

  do {
    cout << msg;
    cin.clear();
    cin.sync();
    getline(cin, inp, '\n');

    if (!func(inp, dep)) {
      cout << on_error;
      if (intentos != 1) { cout << " (" << intentos - 1 << " intentos restantes)"; }
      cout << "\n";
    } else return inp;

    intentos--;
  } while (intentos > 0);

  cout << "Demasiados intentos. Terminando el programa..." << endl;
  exit(0);
}

template<typename T>
T pedirValor(const string &msg, T min, T max) {
  T i;
  string inp;
  bool valid = false;

  do {
    cout << msg;
    cin.clear();
    cin.sync();
    getline(cin, inp, '\n');
    istringstream in(inp);

    in >> i;
    if (i < min || i > max)
      cout << "Error: El valor debe estar en el rango " << min << "..=" << max << endl;
    else valid = true;

  } while (!valid);

  return i;
}


string pedirValor(const string &msg) {
  string inp;

  cout << msg;
  cin.clear();
  cin.sync();
  getline(cin, inp, '\n');


  return inp;
}

void clear();

#endif //ADMIN_UTILS_H
