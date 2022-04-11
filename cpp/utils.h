#ifndef ADMIN_UTILS_H
#define ADMIN_UTILS_H

#define numbers {'0', '1', '2', '3','4','5','6','7','8','9'}
#define numbersc {'0', '1', '2', '3','4','5','6','7','8','9', 'c'}

#include <iostream>
#include <sstream>

using namespace std;

/// Función que elimina todos los caracteres que sean iguales a 'patron'
/// \param txt El string a limpiar de un carácter
/// \param patron El carácter que se desea eliminar
/// \return Una nueva instancia de string que contiene 'txt' con el carácter 'patron' eliminado
string quitar(string &txt, char patron);

/// Función que indica si una cadena de caracteres contiene caracteres autorizados
/// \param valid_chars El vector de caracteres que están permitidos en la entrada
/// \param inp El string a validar
/// \return True si contiene caracteres inválidos
bool contieneInvalid(const vector<char> &valid_chars, const string &inp, string titulo);


/// Realizado con ayuda de https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp/.
/// Función que pide un valor, pensado para strings solamente. La función se ejecuta indefinidamente hasta que
/// la función verificadora que se pasa como argumento devuelva true.
/// \tparam V El tipo de dep
/// \param dep La variable de la que depende que la función verificadora funcione. Pensado para instancias de Banco y Cuenta
/// \param msg El mensaje mostrado al usuario para pedir el dato
/// \param func Un apuntador a una función que devuelva true/false
/// \param on_error El mensaje a mostrar cuando ocurra algún error
/// \param validos El arreglo de caracteres válidos que pueden estar contenidos en el msg
/// \param quitar_espacios Si la función debería ignorar los espacios o no
/// \return El valor ingresado, ya validado por la función func, que depende de dep de tipo V
template<typename V>
string pedirValor(V dep, bool (*func)(string &, V &), const string &on_error, const string& titulo,
                  const vector<char> &validos = {}, bool quitar_espacios = false) {
  string inp;
  bool valid = false;

  do {
    // cout << msg;

    // De acuerdo a https://programmerclick.com/article/626810031/ hay que usar ambos a la vez
    cin.clear();
    cin.sync();

    getline(cin, inp, '\n');

    if (quitar(inp, ' ').empty()) continue;

    if (quitar_espacios) inp = quitar(inp, ' ');

    if (!validos.empty())
      if (contieneInvalid(validos, inp, titulo)) continue;

    if (!func(inp, dep))
      cout << on_error << endl;
    else valid = true;

  } while (!valid);

  return inp;
}


/// Realizado con ayuda de https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp/.
/// Función que pide un valor, pensado para strings solamente. La función se ejecuta indefinidamente hasta que
/// la función verificadora que se pasa como argumento devuelva true. Se pide un valor hasta intentos veces,
/// terminando el programa si se agotan los intentos
/// \tparam V El tipo de dep
/// \param dep La variable de la que depende que la función verificadora funcione. Pensado para instancias de Banco y Cuenta
/// \param msg El mensaje mostrado al usuario para pedir el dato
/// \param func Un apuntador a una función que devuelva true/false
/// \param on_error El mensaje a mostrar cuando ocurra algún error
/// \param intentos El número de intentos permitidos para ingresar un valor válido
/// \param validos El arreglo de caracteres válidos que pueden estar contenidos en el msg
/// \param quitar_espacios Si la función debería ignorar los espacios o no
/// \return El valor ingresado, ya validado por la función func, que depende de dep de tipo V
template<typename V>
string pedirValor(V dep, bool (*func)(string &, V &), const string &on_error, const string& titulo, int intentos,
                  const vector<char> &validos = {}, bool quitar_espacios = false) {
  string inp;
  ofstream file;

  do {
    // cout << msg;

    cin >> inp;

    if (quitar(inp, ' ').empty()) continue;

    if (quitar_espacios) inp = quitar(inp, ' ');

    if (!validos.empty())
      if (contieneInvalid(validos, inp, titulo)) continue;

    file.open("log_ususarios.txt", std::ios::app);
    file << "Verificando...\n";
    file.close();

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

/// Función que pide un valor que será almacenado en una variable de tipo T, pensado exclusivamente para tipos numéricos.
/// El número ingresado debe estar en el rango: min..=max
/// \tparam T El tipo de dato NUMÉRICO donde se almacenará el input de usuario
/// \param msg El mensaje mostrado al usuario para pedir el dato
/// \param min El valor mínimo que debe ser ingresado
/// \param max El valor máximo posible a ser ingresado
/// \return El valor ingresado ya validado
template<typename T>
T pedirValor(T min, T max) {
  T i;
  string inp;
  bool valid = false;

  do {
    // cout << msg;
    cin >> inp;

    if (quitar(inp, ' ').empty()) continue;
    istringstream in(inp);

    in >> i;
    if (i < min || i > max)
      cout << "Error: El valor " << i << " debe estar en el rango " << min << "..=" << max << endl;
    else valid = true;

  } while (!valid);

  return i;
}


/// Función que pide un valor que será almacenado en una variable de tipo T, pensado exclusivamente para tipos numéricos.
/// El número ingresado debe estar en el rango: min..=max
/// \tparam T El tipo de dato NUMÉRICO donde se almacenará el input de usuario
/// \param msg El mensaje mostrado al usuario para pedir el dato
/// \param min El valor mínimo que debe ser ingresado
/// \param max El valor máximo posible a ser ingresado
/// \param quitar_chars Un vector de caracteres a ser ignorados del valor ingresado
/// \param valid_chars Un vector de caracteres que son únicamente válidos
/// \param quitar_espacios Si la función debería ignorar los espacios o no
/// \return El valor ingresado ya validado
template<typename T>
T pedirValor(T min, T max, const vector<char> &quitar_chars, const string& titulo, const vector<char> &valid_chars,
             bool quitar_espacios = false) {
  T i;
  string inp;
  bool valid = false;

  do {
    // cout << msg;
    cin >> inp;

    if (quitar(inp, ' ').empty()) continue;

    if (quitar_espacios) inp = quitar(inp, ' ');

    for (int h = 0; h < quitar_chars.size(); h++) {
      // cout << "Eliminando " << quitar_chars[h] << endl;
      inp = quitar(inp, quitar_chars[h]);
    }

    if (contieneInvalid(valid_chars, inp, titulo)) continue;

    istringstream in(inp);

    in >> i;
    if (i < min || i > max) {
      cout << "<h2 class='text-center'>" << titulo << "r</h2>";
      cout << "<p class='text-center'>Error: El valor debe estar en el rango " << min << "..=" << max
           << "</br>Ingresa la cantidad a transferir:</p>\n";
    } else valid = true;

  } while (!valid);

  return i;
}

/// Pide un string. Usado para tener una forma sencilla de pedir valores usando getline()
/// \param msg El mensaje mostrado al usuario para pedir el dato
/// \param quitar_espacios Si la función debería ignorar los espacios o no
/// \return El valor ingresado
string pedirValor(bool quitar_espacios = false) {
  string inp;

  // cout << msg;
  cin >> inp;

  if (quitar_espacios) inp = quitar(inp, ' ');

  return inp;
}

void clear();

void continuar() {}

void continuar_c() {}

#endif //ADMIN_UTILS_H
