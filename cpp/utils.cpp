//
// Created by Alejandro D on 22/02/22.
//

#include <iostream>
#include <vector>
#include <algorithm>

#include "com.h"

using namespace std;

void clear() {}

/// Función que indica si una cadena de caracteres contiene caracteres autorizados
/// \param valid_chars El vector de caracteres que están permitidos en la entrada
/// \param inp El string a validar
/// \return True si contiene caracteres inválidos
bool contieneInvalid(Communicator & c, const vector<char> &valid_chars, const string &inp) {

  stringstream sout;
  stringstream sin;
  bool invalid = false;

  for (int j = 0; j < inp.size(); j++) {
    if (!count(valid_chars.begin(), valid_chars.end(), inp[j])) {
      if (!invalid) {
        sout << "Error: encontrado: '" << inp[j] << "'";
      } else {
        sout << ", '" << inp[j] << "'";
      }
      invalid = true;

      continue;
    }
  }

  if (invalid) {
    sout << "El valor ingresado puede contener los caracteres: ";
    for (char invalid_char: valid_chars) sout << invalid_char;
    c.send(&sout);
  }


  return invalid;
}

/// Función que elimina todos los caracteres que sean iguales a 'patron'
/// \param txt El string a limpiar de un carácter
/// \param patron El carácter que se desea eliminar
/// \return Una nueva instancia de string que contiene 'txt' con el carácter 'patron' eliminado
string quitar(string &txt, char patron)
{
  string result = string("");
  for (int i = 0; i < txt.length(); i++)
  {
    if (patron != txt[i])
      result += txt[i];
  }
  return result;
}
