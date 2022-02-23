#include "banco.h"

#include <iostream>

// https://stackoverflow.com/questions/997512/string-representation-of-time-t
#include <ctime>

void Cuenta::agregarDinero(double cantidad) {
  dinero += cantidad;
}

double Cuenta::retirarDinero(double cantidad) {
  dinero -= cantidad;
  return dinero;
}

void Cuenta::ver_cuenta() {
  cout << nombre << " " << apellido << ": " << endl;
  cout << "    Tarjeta: ";


  cout << tarjeta.substr(0, 4) << " ";
  cout << tarjeta.substr(4, 4) << " ";
  cout << tarjeta.substr(8, 4) << " ";
  cout << tarjeta.substr(12, 4);
  cout << " (" << nip << ")" << endl;
  // https://stackoverflow.com/questions/997512/string-representation-of-time-t
  std::tm *ptm = std::localtime(&fecha_vencimiento);
  char buffer[32];

  std::strftime(buffer, 32, "%m/%Y", ptm);

  cout << "    Teléfono: ";
  cout << tel.substr(0, 2) << " " << tel.substr(2, tel.length()) << endl;
  cout << "    FV: " << buffer << "\n";
  cout << "    Valid True" << endl;
}

bool Cuenta::validar_contra(const string &contra) {
  return contra == key;
}

void Cuenta::verSaldo() {
  std::cout << apellido << ", " << nombre << ": " << endl
            << "    Saldo: " << dinero << endl
            << "    Deuda: " << deuda << endl
            << "    Total: " << dinero - deuda << endl;
}

void Cuenta::verCuentas() {
  if (tarjetas_registradas.empty()) {
    cout << "No hay cuentas registradas!" << endl;
    cout << "Nota: Puedes registrar cuentas en Transferencias/Registrar cuentas\n";
  } else {
    cout << "Cuentas registradas para transferencia:\n";
    for (int i = 0; i < tarjetas_registradas.size(); i++) {
      cout << "    " << i + 1 << ": "
           << tarjetas_registradas[i].substr(0, 4) << " "
           << tarjetas_registradas[i].substr(4, 4) << " "
           << tarjetas_registradas[i].substr(8, 4) << " "
           << tarjetas_registradas[i].substr(12, 4) << "\n";
    }
  }
}

bool Cuenta::validar_super_contra(const string &contra) {
  return super_key == contra;
}

void Cuenta::eliminarCuenta_t(const string &t) {
  for (int i = 0; i < tarjetas_registradas.size(); i++) {
    if (tarjetas_registradas[i] == t) {
      // https://stackoverflow.com/questions/875103/how-do-i-erase-an-element-from-stdvector-by-index
      tarjetas_registradas.erase(std::next(tarjetas_registradas.begin(), i - 1));
      return;
    }
  }

  cout << "La cuenta no existe en el registro\n";
}

void Cuenta::registrarCuenta_t(const string &t) {
  if (t == "c") return;

  if (cuentaYaRegistrada_t(t)) {
    cout << "Error: La cuenta ya está registrada\n";
  } else {
    tarjetas_registradas.push_back(t);
    cout << "Cuenta registrada con éxito!\n";
  }
}

bool Cuenta::cuentaYaRegistrada_t(const string &t) {
  return count(tarjetas_registradas.begin(), tarjetas_registradas.end(), t);
}

Cuenta Banco::buscarCuenta() {
  string cuenta_str;
  ResB cuenta;
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

ResB Banco::buscarCuentaRaw(const string &tarjeta) {
  ResB resultado = ResB();
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
  ResB resultado = banco.buscarCuentaRaw(cuenta);

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

bool esSuperKey_s(string &key, Cuenta cuenta) {
  return cuenta.validar_super_contra(key);
}

bool cuentaExisteOCancela_s(string &cuenta, Banco banco) {
  ResB resultado = banco.buscarCuentaRaw(cuenta);

  return resultado.fue_exitosa || cuenta == "cancelar" || cuenta == "c";
}
