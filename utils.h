#ifndef ADMIN_UTILS_H
#define ADMIN_UTILS_H

#define numbers {'0', '1', '2', '3','4','5','6','7','8','9'}
#define numbersc {'0', '1', '2', '3','4','5','6','7','8','9', 'c'}

#include <iostream>
#include <sstream>

using namespace std;

string quitar(string &txt, char patron);

bool contieneInvalid(const vector<char> &valid_chars, const string &inp);

// Con ayuda de https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp/
template<typename V>
string pedirValor(V dep, const string &msg, bool (*func)(string &, V &), const string &on_error,
                  const vector<char> &validos = {}, bool quitar_espacios = false) {
  string inp;
  bool valid = false;

  do {
    cout << msg;

    // de acuerdo a https://programmerclick.com/article/626810031/
    cin.clear();
    cin.sync();
    getline(cin, inp, '\n');

    if (quitar(inp, ' ').empty()) continue;

    if (quitar_espacios) inp = quitar(inp, ' ');

    if (!validos.empty())
      if (contieneInvalid(validos, inp)) continue;

    if (!func(inp, dep))
      cout << on_error << endl;
    else valid = true;

  } while (!valid);

  return inp;
}


template<typename V>
string pedirValor(V dep, const string &msg, bool (*func)(string &, V &), const string &on_error, int intentos,
                  const vector<char> &validos = {}, bool quitar_espacios = false) {
  string inp;

  do {
    cout << msg;
    cin.clear();
    cin.sync();
    getline(cin, inp, '\n');

    if (quitar(inp, ' ').empty()) continue;

    if (quitar_espacios) inp = quitar(inp, ' ');

    if (!validos.empty())
      if (contieneInvalid(validos, inp)) continue;

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

    if (quitar(inp, ' ').empty()) continue;
    istringstream in(inp);

    in >> i;
    if (i < min || i > max)
      cout << "Error: El valor debe estar en el rango " << min << "..=" << max << endl;
    else valid = true;

  } while (!valid);

  return i;
}

template<typename T>
T pedirValor(const string &msg, T min, T max, const vector<char> &quitar_chars) {
  T i;
  string inp;
  bool valid = false;

  do {
    cout << msg;
    cin.clear();
    cin.sync();
    getline(cin, inp, '\n');

    if (quitar(inp, ' ').empty()) continue;

    for (int i = 0; i < quitar_chars.size(); i++) {
      cout << "Eliminando " << quitar_chars[i] << endl;
      inp = quitar(inp, quitar_chars[i]);
    }

    istringstream in(inp);

    in >> i;
    if (i < min || i > max)
      cout << "Error: El valor debe estar en el rango " << min << "..=" << max << endl;
    else valid = true;

  } while (!valid);

  return i;
}


template<typename T>
T pedirValor(const string &msg, T min, T max, const vector<char> &quitar_chars, const vector<char> &valid_chars,
             bool quitar_espacios = false) {
  T i;
  string inp;
  bool valid = false;

  do {
    cout << msg;
    cin.clear();
    cin.sync();
    getline(cin, inp, '\n');

    if (quitar(inp, ' ').empty()) continue;

    if (quitar_espacios) inp = quitar(inp, ' ');

    for (int h = 0; h < quitar_chars.size(); h++) {
      // cout << "Eliminando " << quitar_chars[h] << endl;
      inp = quitar(inp, quitar_chars[h]);
    }

    if (contieneInvalid(valid_chars, inp)) continue;

    istringstream in(inp);

    in >> i;
    if (i < min || i > max)
      cout << "Error: El valor debe estar en el rango " << min << "..=" << max << endl;
    else valid = true;

  } while (!valid);

  return i;
}

string pedirValor(const string &msg, bool quitar_espacios = false) {
  string inp;

  cout << msg;
  cin.clear();
  cin.sync();
  getline(cin, inp, '\n');

  if (quitar_espacios) inp = quitar(inp, ' ');

  return inp;
}

void clear();

void continuar() {
  pedirValor("Pulsa enter para continuar");
}

void continuar_c() {
  pedirValor("Pulsa enter para continuar");
  clear();
}

#endif //ADMIN_UTILS_H
