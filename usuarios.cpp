#include <iostream>

#include "banco.h"
#include "utils.h"

#define SALIR 6

int menuPrincipal(Cuenta &cuenta);

int menuTransferencias(Cuenta &cuenta);

int menuEstado(Cuenta &cuenta);

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
    op = menuPrincipal(cuenta);
  } while (op != SALIR);

}

int menuPrincipal(Cuenta &cuenta) {
  clear();
  printf(
      "Súper Linea -- Super Banco\n"
      "Submenús:\n"
      "    1. Estado\n"
      "(*) 2. Transferencias\n"
      "(*) 3. Retirar o depositar\n"
      "(*) 4. Préstamos\n"
      "    5. Información\n"
      "    6. Salir\n"
      "\n(*) : Puede requerir su Super-key\n"
  );

  int option = pedirValor("Ingrese un submenú: ", 1, SALIR);
  int op;
  switch (option) {
    case 1:
      do { op = menuEstado(cuenta); }
      while (op != 4);
      break;
    case 2:
      do { op = menuTransferencias(cuenta); }
      while (op != 5);
      break;
  }

  return option;
}


int menuEstado(Cuenta &cuenta) {
  clear();
  printf(
      "    1. Ver saldo\n"
      "    2. Ver datos personales\n"
      "    3. Ver cuentas registradas\n"
      "    4. Regresar\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, 4);

  switch (option) {
    case 1:
      cuenta.verSaldo();
      break;
    case 2:
      cuenta.ver_cuenta();
      break;
    case 3:
      cuenta.verCuentas();
      break;
    default:
      return option;
  }

  continuar();
  return option;
}

int menuTransferencias(Cuenta &cuenta) {
  clear();
  printf(
      "    1. Transferir a tarjeta\n"
      "    2. Registrar cuentas\n"
      "    3. Editar cuentas\n"
      "    4. Ver cuentas registradas\n"
      "    5. Regresar\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, 5);

  switch (option) {
    case 1:
      cuenta.verSaldo();
      break;
    case 2:
      cuenta.ver_cuenta();
      break;
    case 3:
      cuenta.verCuentas();
      break;
    default:
      return option;
  }

  continuar();
  return option;
}

