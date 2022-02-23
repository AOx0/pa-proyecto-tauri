#include "banco.h"

#include <iostream>

// https://stackoverflow.com/questions/997512/string-representation-of-time-t
#include <ctime>

void Cuenta::agregarDinero(double cantidad) {
  dinero += cantidad;
}

double Cuenta::retirarDinero(double cantidad) {
  dinero -= cantidad;
  return cantidad;
}

void Cuenta::ver_cuenta() {
  cout << nombre << " " << apellido << endl;
  cout << "Tarjeta: ";


  cout << tarjeta.substr(0, 4) << " ";
  cout << tarjeta.substr(4, 4) << " ";
  cout << tarjeta.substr(8, 4) << " ";
  cout << tarjeta.substr(12, 4) << "\n";
  cout << "Nip: " << nip << endl;
  // https://stackoverflow.com/questions/997512/string-representation-of-time-t
  std::tm *ptm = std::localtime(&fecha_vencimiento);
  char buffer[32];

  std::strftime(buffer, 32, "%m/%Y", ptm);

  cout << "Teléfono: ";
  cout << tel.substr(0, 2) << " " << tel.substr(2, tel.length()) << endl;
  cout << "FV: " << buffer << "\n";
  cout << "Valid True" << endl;
}

bool Cuenta::validar_contra(const string &contra) {
  if (contra == key) {
    cout << "Contraseña valida" << endl;
    return true;
  }
  return false;
}

void Cuenta::verSaldo() {
  std::cout << apellido << ", " << nombre << ": " << endl
            << "    Saldo: " << dinero << endl
            << "    Deuda: " << deuda << endl
            << "----------------" << endl
            << "Restante : " << dinero - deuda << endl;
}

void Cuenta::verCuentas() {
  if (tarjetas_registradas.empty()) {
    cout << "No hay cuentas registradas!" << endl;
    cout << "Nota: Puedes registrar cuentas en Transferencias/Registrar cuentas\n";
  } else {
    for (int i = 0; i < tarjetas_registradas.size(); i++) {
      cout << "    " << i << ". " << tarjetas_registradas[i] << "\n";
    }
  }
}

Cuenta Banco::buscarCuenta() {
  string cuenta_str;
  ResultadoB cuenta;
  do {
    cout << "Ingresa un número de tarjeta: ";
    cin >> cuenta_str;
    cuenta = buscarCuentaRaw(cuenta_str);

    if (cuenta.fue_exitosa) {
      return *cuenta.encontrada;
    } else {
      cout << "Cuenta no encontrada\n";
    }
  } while (!cuenta.fue_exitosa);

  return *cuenta.encontrada;
}

ResultadoB Banco::buscarCuentaRaw(const string &tarjeta) {
  ResultadoB resultado = ResultadoB();
  for (Cuenta &cuenta: cuentas) {
    if (tarjeta == cuenta.tarjeta) {
      resultado.fue_exitosa = true;
      resultado.encontrada = &cuenta;
      return resultado;
    }
  }
  return resultado;
}


bool contraEs_s(string &contra, Cuenta cuenta) {
  return cuenta.validar_contra(contra);
}

bool cuentaExiste_s(string &cuenta, Banco banco) {
  ResultadoB resultado = banco.buscarCuentaRaw(cuenta);

  return resultado.fue_exitosa;
}

vector<Cuenta> cuentas_iniciales() {
  return vector<Cuenta>(
      {
          Cuenta(
              500, "123", "Pepe",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "1435", "1234567890123456",
              "432423234", 234, "5574481902", "López", {}
          ),
          Cuenta(
              500, "123", "Pepe",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "1435", "3334567890123456",
              "432423234", 234, "5574481902", "López", {}
          ),
          Cuenta(
              500, "123", "Pepe",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "1435", "3334567890123456",
              "432423234", 234, "5574481902", "López", {}
          ),
          Cuenta(
              500, "123", "Pepe",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "1435", "3334567890123456",
              "432423234", 234, "5574481902", "López", {}
          ),
          Cuenta(
              500, "123", "Pepe",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "1435", "3334567890123456",
              "432423234", 234, "5574481902", "López", {}
          ),
      }
  );
}
