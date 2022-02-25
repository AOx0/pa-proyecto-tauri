#include <iostream>

#include "banco.h"
#include "utils.h"

#define SALIR 5

int menuPrincipal(Cuenta &cuenta, Banco &banco);

int menuTransferencias(Cuenta &cuenta, Banco &banco);

int menuEstado(Cuenta &cuenta, Banco &banco);

int menuDepRet(Cuenta &cuenta, Banco &banco);

int menuPrestamos(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "Opciones: \n"
      "    1. Pagar deuda\n"
      "    2. Solicitar préstamo\n"
      "    3. Ver saldo\n"
      "    4. Regresar\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, 4);

  clear();
  switch (option) {
    case 1: {
      if (cuenta.deuda == 0) {
        cout << "No cuentas con una deuda que pagar.\n";
        break;
      }

      string key = pedirValor<Cuenta>(
          cuenta, "Ingresa tu Super-Key: ",
          &esSuperKey_s, "Super-Key inválida", 3
      );
      clear();

      cout << "Deuda actual: $" << cuenta.deuda << endl;
      double cantidad = pedirValor("Ingresa la cantidad a pagar: ", 1.0, cuenta.deuda, {','}, numbers, true);

      if (cantidad > cuenta.dinero) {
        cout << "Error: No cuentas con los fondos suficientes para pagar la deuda\n";
      } else {
        clear();
        cuenta.retirarDinero(cantidad);
        cuenta.deuda -= cantidad;
        cout << "Éxito, resultado de la operación: \n"
              << "Deuda: " << cuenta.deuda+cantidad << " -> " << cuenta.deuda << endl
              << "Saldo: " << cuenta.dinero+cantidad << " -> " << cuenta.dinero << endl;
      }

    }
      break;
    case 2:
      cuenta.verCuenta();
      break;
    case 3:
      cuenta.verSaldo();
      break;
    default:
      return option;
  }

  continuar_c();
  return option;
}

int main() {
  clear();
  Banco banco = Banco(cuentas_iniciales());

  string tarjeta = pedirValor<Banco>(
      banco, "Ingresa tu tarjeta: ",
      &cuentaExiste_s, "Tarjeta inválida", numbers, true
  );
  Cuenta cuenta = *banco.buscarCuentaRaw(tarjeta).encontrada;

  string contra = pedirValor<Cuenta>(
      cuenta, "Ingresa la contraseña: ",
      &contraEs_s, "Contraseña inválida", 3, numbers
  );

  while (menuPrincipal(cuenta, banco) != SALIR) {}
}

int menuPrincipal(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "Súper Linea -- Super Banco\n"
      "--------------------------\n"
      "Submenús:\n"
      "    1. Ver estado\n" // Ya
      "(*) 2. Transferencias\n"
      "(*) 3. Retirar o depositar\n" // Ya
      "(*) 4. Préstamos\n"
      "    5. Salir\n"
      "\n(*) : Puede requerir su Super-key\n"
  );

  int option = pedirValor("Ingrese un submenú: ", 1, SALIR);
  switch (option) {
    case 1:
      while (menuEstado(cuenta, banco) != 4) {}
      break;
    case 2:
      while (menuTransferencias(cuenta, banco) != 6) {}
      break;
    case 3:
      while (menuDepRet(cuenta, banco) != 4) {}
      break;
    case 4:
      while (menuPrestamos(cuenta, banco) != 4) {}
      break;
  }

  return option;
}

int menuEstado(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "Opciones: \n"
      "    1. Ver saldo\n"
      "    2. Ver datos personales\n"
      "    3. Ver cuentas registradas\n"
      "    4. Regresar\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, 4);

  clear();
  switch (option) {
    case 1:
      cuenta.verSaldo();
      break;
    case 2:
      cuenta.verCuenta();
      break;
    case 3:
      cuenta.verCuentas();
      break;
    default:
      return option;
  }

  continuar_c();
  return option;
}

int menuTransferencias(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "Opciones: \n"
      "(*) 1. Transferir a tarjeta\n"
      "(*) 2. Registrar cuenta\n"
      "(*) 3. Eliminar cuenta\n"
      "    4. Ver cuentas registradas\n"
      "    5. Ver saldo\n"
      "    6. Regresar\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, 6);

  if (option < 4 && option > 1) {
    cout << "La operación requiere Super-Key\n";
    string key = pedirValor<Cuenta>(
        cuenta, "Ingresa tu Super-Key: ",
        &esSuperKey_s, "Super-Key inválida", 3
    );
    clear();
  }

  clear();

  bool cancelado = false;
  switch (option) {
    case 1: {
      string a_transferir;
      bool valido;

      do {
        cout << "Nota: Puedes cancelar la operación ingresando 'c'\n";
        a_transferir = pedirValor<Banco>(
            banco, "Ingresa la tarjeta a transferir: ",
            &cuentaExisteOCancela_s, "Tarjeta inválida", numbersc, true
        );

        if (a_transferir == "c") {
          cancelado = true;
          break;
        }

        if (cuenta.tarjeta == a_transferir) {
          cout << "Tarjeta inválida. No puedes transferir dinero a ti mismo!\n";
        } else {
          valido = true;
        }
      } while (!valido);

      if (cancelado) {
        cout << "Operación cancelada\n";
        break;
      }

      double cantidad = pedirValor("Ingresa la cantidad a transferir: ", 100.0, 5000000.0, {','}, numbers, true);

      if (!count(cuenta.tarjetas_registradas.begin(), cuenta.tarjetas_registradas.end(), a_transferir)) {
        clear();
        cout << "Se requiere Super-Key para cuentas no registradas.\n";
        string key = pedirValor<Cuenta>(
            cuenta, "Ingresa tu Super-Key: ",
            &esSuperKey_s, "Super-Key inválida", 3
        );
        clear();
      }

      if (pedirValor("Ingresa cualquier cosa para continuar o ingresa 'c' para cancelar ", true) == "c") {
        cancelado = true;
        break;
      }

      Cuenta &target = (Cuenta &) banco.buscarCuentaRaw(a_transferir).encontrada;

      if (cantidad > cuenta.dinero) {
        cout << "Error: No hay suficientes fondos para realizar la transferencia\n";
      } else {
        cuenta.transferir(target, cantidad);
        double restante = cuenta.dinero;
        cout << "Éxito. Se transfirieron $" << cantidad << " MXN\n";
        cout << "Saldo restante: $" << restante << ".00\n";
      }

    }
      break;
    case 2: {
      cout << "Al registrar puedes transferir sin usar Super-Key.\n";
      cout << "Ingresa \"c\" para cancelar\n";

      string a_agregar;
      bool valido = false;
      do {
        a_agregar = pedirValor<Banco>(
            banco, "Ingresa la tarjeta a registrar: ",
            &cuentaExisteOCancela_s, "Tarjeta inválida", numbersc, true
        );

        if (a_agregar == "c") {
          cancelado = true;
          break;
        }

        if (cuenta.tarjeta == a_agregar) {
          cout << "Tarjeta inválida. No puedes registrarte a ti mismo!\n";
        } else {
          valido = true;
        }
      } while (!valido);

      if (!cancelado) {
        cuenta.registrarCuenta_t(a_agregar);
      } else {
        cout << "Operación cancelada\n";
      }
    }
      break;
    case 3: {
      cuenta.verCuentas();

      if (cuenta.tarjetas_registradas.empty()) break;

      cout << "Ingresa \"c\" para cancelar\n";

      string a_borrar;
      bool valido = false;
      do {
        a_borrar = pedirValor<Banco>(
            banco, "Ingresa la tarjeta a eliminar: ",
            &cuentaExisteOCancela_s, "Tarjeta inválida", numbersc, true
        );

        if (a_borrar == "c") {
          cancelado = true;
          break;
        } else {
          if (!std::count(cuenta.tarjetas_registradas.begin(), cuenta.tarjetas_registradas.end(), a_borrar)) {
            cout << "Tarjeta inválida. No está registrada como tarjeta de transferencia verificada.\n";
          } else if (cuenta.tarjeta == a_borrar) {
            cout << "Tarjeta inválida. No puedes registrarte a ti mismo!\n";
          } else {
            valido = true;
          }
        }
      } while (!valido);

      if (!cancelado) {
        cuenta.eliminarCuenta_t(a_borrar);
        cout << "Cuenta eliminada con éxito!\n";
      } else {
        cout << "Operación cancelada\n";
      }
    }
      break;
    case 4:
      cuenta.verCuentas();
      break;
    case 5:
      cuenta.verSaldo();
      break;
    default:
      return option;
  }

  continuar_c();
  return option;
}

int menuDepRet(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "Opciones: \n"
      "    1. Ver saldo\n"
      "    2. Depositar\n"
      "(*) 3. Retirar\n"
      "    4. Regresar\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, 4);
  double cantidad;

  clear();
  switch (option) {
    case 1:
      cuenta.verSaldo();
      break;
    case 2:
      cantidad = pedirValor("Ingresa la cantidad a depositar: ", 100, 50000, {','}, numbers, true);
      cuenta.agregarDinero(cantidad);
      cout << "Ingresa el dinero en la bandeja.\n";
      continuar();
      cout << "Éxito. Se depositaron: $" << cantidad << " MXN\n";
      break;
    case 3:
      cantidad = pedirValor("Ingresa la cantidad a retirar: ", 100, 50000, {','}, numbers, true);

      if (cantidad > 2500) {
        clear();
        cout << "Una cantidad mayor de 2500 (" << cantidad << ") requiere Super-Key.\n";
        string key = pedirValor<Cuenta>(
            cuenta, "Ingresa tu Super-Key: ",
            &esSuperKey_s, "Super-Key inválida", 3
        );
        clear();
      }

      if (cantidad > cuenta.dinero) {
        cout << "Error: No hay suficientes fondos\n";
      } else {
        double restante = cuenta.retirarDinero(cantidad);
        cout << "Éxito. Se retiraron $" << cantidad << " MXN\n";
        cout << "Saldo restante: $" << restante << ".00\n";
        cout << "Recoge el dinero de la bandeja.\n";
      }
      break;
    default:
      return option;
  }

  continuar_c();
  return option;
}

