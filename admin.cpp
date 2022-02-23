//
// Created by Alejandro D on 19/02/22.
//

#include <iostream>
#include "banco.h"


int main() {
  Banco banco = Banco(cuentas_iniciales());
  Cuenta cuenta = banco.buscarCuenta();

  int opcion;
  cout << "Ingrese una opción: \n"
       << "    1- Ver cuenta\n";
  cout << "Opción: ";
  cin >> opcion;

  switch (opcion) {
    case 1:
      cuenta.ver_cuenta();
    case 2:
      break;
    default:
      std::cout << "Opción valida\n";
  }

}