#include <iostream>
#include <fstream>

#include "banco.h"
#include "utils.h"

#define SALIR 50

// El menú principal. Lo muestra y se encarga de pedir una opción y ejecutarla
int menuPrincipal(Cuenta &cuenta, Banco &banco);

// El menú de transferencias. Lo muestra y se encarga de pedir una opción y ejecutarla
int menuTransferencias(Cuenta &cuenta, Banco &banco);

// El menú de estado. Lo muestra y se encarga de pedir una opción y ejecutarla
int menuEstado(Cuenta &cuenta, Banco &banco);

// El menú de depósitos/retiros. Lo muestra y se encarga de pedir una opción y ejecutarla
int menuDepRet(Cuenta &cuenta, Banco &banco);

// El menú de préstamos. Lo muestra y se encarga de pedir una opción y ejecutarla
int menuPrestamos(Cuenta &cuenta, Banco &banco);

int main() {
  clear();
  Banco banco = Banco(cuentas_iniciales());

  while (true) {
    string tarjeta = pedirValor<Banco>(
      banco, "Ingresa tu tarjeta: ",
      &cuentaExiste_s, "<h2 class='text-center'>Login</h2><p class='text-center'>Tarjeta inválida</p>", numbers, true
    );

    Cuenta cuenta = *banco.buscarCuentaRaw(tarjeta).encontrada;

    cout << "<h2 class='text-center'>Login</h2><p class='text-center'>Ahora ingresa la contraseña</p>\n";

    string contra = pedirValor<Cuenta>(
        cuenta, "Ingresa la contrasenna: ",
        &contraEs_s, "<h2 class='text-center'>Login</h2><p class='text-center'>Contraseña inválida</p>", 3, numbers
    );

    while (menuPrincipal(cuenta, banco) != SALIR) {}

    cout << "<h2 id='re' class='text-center'>Login</h2><p class='text-center'>Ingresa tu número de tarjeta</p>\n";
  }




}

int menuPrincipal(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "<h2 class='text-center'>Súper Linea</h2>:nl:"
      "<p class='text-center'>Submenús:nl:Opciones con el símbolo * pueden requerir su Super-key</p>"
      "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Ver estado</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no2' class='btn btn-primary'>Transferencias (*)</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no3' class='btn btn-primary'>Retirar o depositar (*)</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no4' class='btn btn-primary'>Préstamos (*)</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no5' class='btn btn-primary'>Salir</button></div>"
      "</p>\n"
  );

  int option = pedirValor("Ingrese un submenú: ", 1, SALIR), val;

  switch (option) {
    case 1:
      while ((val = menuEstado(cuenta, banco)) != 4 && val != SALIR) {}
      break;
    case 2:
      while ((val =menuTransferencias(cuenta, banco)) != 6 && val != SALIR) {}
      break;
    case 3:
      while ((val =menuDepRet(cuenta, banco)) != 4 && val != SALIR) {}
      break;
    case 4:
      while ((val =menuPrestamos(cuenta, banco)) != 4 && val != SALIR) {}
      break;
    default:
      return SALIR;
  }

  if (val == SALIR) {
    return SALIR;
  }

  return option;
}

int menuEstado(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "<h2 class='text-center'>Menu Estado</h2>"
      "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Ver saldo</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no2' class='btn btn-primary'>Ver datos personales</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no3' class='btn btn-primary'>Ver cuentas registradas</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='back' class='btn btn-primary'>Regresar</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no5' class='btn btn-primary'>Salir</button></div>"
      "</p>\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, SALIR);


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
    case 4:
      return option;
    case 5:
      return SALIR;
  }

  continuar_c();
  return option;
}

int menuTransferencias(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "<h2 class='text-center'>Menu Transferencias</h2>"
      "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Transferir a tarjeta (*)</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no2' class='btn btn-primary'>Registrar cuenta (*)</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no3' class='btn btn-primary'>Eliminar cuenta (*)</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no4' class='btn btn-primary'>Ver cuentas registradas</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no5' class='btn btn-primary'>Ver saldo</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='back' class='btn btn-primary'>Regresar</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no7' class='btn btn-primary'>Salir</button></div>"
      "</p>\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, SALIR);


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
    case 6:
      return option;
    case 7:
      return SALIR;
  }

  continuar_c();
  return option;
}

int menuDepRet(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "<h2 class='text-center'>Menu Depositos & Retiros</h2>"
      "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Ver saldo</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no2' class='btn btn-primary'>Depositar</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no3' class='btn btn-primary'>Retirar (*)</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='back' class='btn btn-primary'>Regresar</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no5' class='btn btn-primary'>Salir</button></div>"
      "</p>\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, SALIR);


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
    case 4:
      return option;
    case 5:
      return SALIR;
  }

  continuar_c();
  return option;
}

int menuPrestamos(Cuenta &cuenta, Banco &banco) {
  clear();
  printf(
      "<h2 class='text-center'>Menu Prestamos</h2>"
      "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Pagar deuda</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no2' class='btn btn-primary'>Solicitar préstamo</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no3' class='btn btn-primary'>Ver saldo (*)</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='back' class='btn btn-primary'>Regresar</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no5' class='btn btn-primary'>Salir</button></div>"
      "</p>\n"
  );

  int option = pedirValor("Ingrese una opción: ", 1, SALIR);


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
             << "Deuda: " << cuenta.deuda + cantidad << " -> " << cuenta.deuda << endl
             << "Saldo: " << cuenta.dinero + cantidad << " -> " << cuenta.dinero << endl;
      }

    }
      break;
    case 20: {
      string key = pedirValor<Cuenta>(
          cuenta, "Ingresa tu Super-Key: ",
          &esSuperKey_s, "Super-Key inválida", 3
      );
      clear();

      cout << "Saldo actual: $" << cuenta.dinero << endl;
      cout << "Deuda actual: $" << cuenta.deuda << endl;
      double cantidad = pedirValor("Ingresa la cantidad que desea solicitar: ", 1.0, cuenta.dinero * 0.7, {','}, numbers, true);

      if (cantidad/cuenta.dinero > 0.7) {
        cout << "No se puede solicitar más del 70% de la cantidad disponible de saldo\n";
      } else if (cuenta.deuda/cuenta.dinero > 0.15) {
        cout << "No puedes solicitar préstamos si tienes ya una gran cantidad de deuda por pagar\n";
      } else {
        cuenta.deuda += cantidad;
        cuenta.dinero += cantidad;

        cout << "Préstamo solicitado con éxito. Resultado de la operación: \n"
             << "Deuda: " << cuenta.deuda - cantidad << " -> " << cuenta.deuda << endl
             << "Saldo: " << cuenta.dinero - cantidad << " -> " << cuenta.dinero << endl;
      }
    }
      break;
    case 3:
      cuenta.verSaldo();
      break;
    case 4:
      return option;
    case 5:
      return SALIR;
  }

  continuar_c();
  return option;
}

