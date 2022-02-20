//
// Created by Alejandro D on 19/02/22.
//

#include <iostream>
#include "library.h"


int main() {
  vector<Cuenta> cuentas = vector<Cuenta>(
      {
          Cuenta(
              500, string("123"), string("Pepe"),
              time(nullptr), vector<double>({50, 50, 50, 50}),
              {3, 4, 4}, string("1234567890123456"),
              string("4324234")
          ),
          Cuenta(
              500, string("123"), string("Pepe"),
              time(nullptr), vector<double>({50, 50, 50, 50}),
              {3, 4, 4}, string("1234567890143456"),
              string("4324234")
          ),
          Cuenta(
              500, string("123"), string("Pepe"),
              time(nullptr), vector<double>({50, 50, 50, 50}),
              {3, 4, 4}, string("1234567890423456"),
              string("4324234")
          ),
          Cuenta(
              500, string("123"), string("Pepe"),
              time(nullptr), vector<double>({50, 50, 50, 50}),
              {3, 4, 4}, string("1234563890123456"),
              string("4324234")
          ),
          Cuenta(
              500, string("123"), string("Pepe"),
              time(nullptr), vector<double>({50, 50, 50, 50}),
              {3, 4, 4}, string("1234567844423456"),
              string("4324234")
          )
      }
  );

  Banco banco = Banco(cuentas);
  Cuenta cuenta = banco.buscarCuenta();

  int opcion;
  cout<<"Ingrese una opción: \n"
      << "    1- Ver cuenta\n";
  cout << "Opción: ";
  cin>>opcion;

  switch (opcion)
  {
    case 1:
      cuenta.ver_cuenta();
    case 2:
      break;
    default:
      std::cout << "Opción valida\n";
  }
}