#include <iostream>

#include "banco.h"
#include "utils.h"

#define SALIR 6

int menuPrincipal() {
  clear();
  printf(
      "    1. Estado\n"
      "    2. Transferencias\n"
      "    3. Retirar o depositar\n"
      "    5. Préstamos\n"
      "    6. Salir\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, SALIR);

  switch (option) {
    case 1: {
    }
      break;
    case 2:
      break;
  }

  return option;
}

void menuEstado() {

}

int main() {
  Banco banco = Banco(cuentas_iniciales());

  string tarjeta = pedirValor<Banco>(
      banco, "Ingresa tu tarjeta: ",
      &cuentaExiste_s, "Tarjeta inválida"
  );
  Cuenta cuenta = *banco.buscarCuentaRaw(tarjeta).encontrada;

  string contra = pedirValor<Cuenta>(
      cuenta, "Ingresa la contraseña: ",
      &contraEs_s, "Contraseña inválida", 3
  );

  int op;
  do {
    op = menuPrincipal();
  } while (op != SALIR);

}

