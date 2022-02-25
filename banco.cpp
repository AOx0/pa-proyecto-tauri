#include "banco.h"

#include <iostream>

// https://stackoverflow.com/questions/997512/string-representation-of-time-t
#include <ctime>

/// Suma al saldo de sí mismo la cantidad ingresada
/// \param cantidad La cantidad a agregar
void Cuenta::agregarDinero(double cantidad) {
  dinero += cantidad;
}

/// Disminuye la cantidad al saldo y devuelve el saldo restante
/// \param cantidad La cantidad a restar
/// \return El valor del saldo restante
double Cuenta::retirarDinero(double cantidad) {
  dinero -= cantidad;
  return dinero;
}

/// Muestra la información del usuario
void Cuenta::verCuenta() {
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


/// Método que verifica que una cadena sea igual a la contraseña registrada
/// \param contra El string, intento de contraseña a verificar
/// \return Devuelve true si es correcta
bool Cuenta::validarContra(const string &contra) const {
  return contra == key;
}

/// Muestra el saldo del usuario
void Cuenta::verSaldo() const {
  std::cout << apellido << ", " << nombre << ": " << endl
            << "    Saldo: " << dinero << endl
            << "    Deuda: " << deuda << endl
            << "    Total: " << dinero - deuda << endl;
}

/// Muestra las cuentas registradas para transferencias verificadas
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


/// Método que verifica que una cadena sea igual a la super-contraseña registrada
/// \param contra El string, intento de contraseña a verificar
/// \return Devuelve true si es correcta
bool Cuenta::validarSuperContra(const string &contra) const {
  return super_key == contra;
}

/// Elimina t del vector de tarjetas registradas
/// \param t La tarjeta que se desea registrar
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

/// Registra t como cuenta de transferencia verificada
/// \param t La tarjeta que se desea registrar
void Cuenta::registrarCuenta_t(const string &t) {
  if (t == "c") return;

  if (cuentaYaRegistrada_t(t)) {
    cout << "Error: La cuenta ya está registrada\n";
  } else {
    tarjetas_registradas.push_back(t);
    cout << "Cuenta registrada con éxito!\n";
  }
}


/// Verifica si una tarjeta dada, en forma de string, está ya registrada como cuenta de transferencia verificada
/// \param t La tarjeta que se desea conocer si se encuentra ya registrada
/// \return true si ya se encuentra registrada
bool Cuenta::cuentaYaRegistrada_t(const string &t) {
  return count(tarjetas_registradas.begin(), tarjetas_registradas.end(), t);
}

void Cuenta::transferir(Cuenta &target, double cantidad) {
  dinero -= cantidad;
  target.dinero += cantidad;

  if (1) {
    cout << "    Tu dinero: " << dinero + cantidad << "->" << dinero << "\n";
    cout << "    Su dinero: " << target.dinero - cantidad << "->" << target.dinero << "\n";
  }
}

/// Método que busca dentro de las cuentas del banco la cuenta dada (tarjeta)
/// \param cuenta El srting con el número de cuenta (tarjeta) a buscar
/// \return Un resultado que contiene el estatus de la búsqueda y en caso de ser exitosa un apuntador a la instancia de Cuenta
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


bool contraEs_s(string &contra, Cuenta & cuenta) {
  return cuenta.validarContra(contra);
}

bool cuentaExiste_s(string &cuenta, Banco & banco) {
  ResB resultado = banco.buscarCuentaRaw(cuenta);

  return resultado.fue_exitosa;
}

bool esSuperKey_s(string &key, Cuenta & cuenta) {
  return cuenta.validarSuperContra(key);
}

bool cuentaExisteOCancela_s(string &cuenta, Banco & banco) {
  ResB resultado = banco.buscarCuentaRaw(cuenta);

  return resultado.fue_exitosa || cuenta == "cancelar" || cuenta == "c";
}


vector<Cuenta> cuentas_iniciales() {
  return vector<Cuenta>(
      {
          Cuenta(
              550000, "323", "Victor",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "1435", "5281713025041549",
              "83740285", 0, "5537930472", "López", {"5851295645900890"}
          ),
          Cuenta(
              79800, "545", "David",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "2344", "5851295645900890",
              "55682904", 15000, "5598527366", "Perez", {}
          ),
          Cuenta(
              155000, "646", "Sara",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "5453", "5369580086986788",
              "23480418", 3349, "5573638103", "Perez", {}
          ),
          Cuenta(
              345000, "836", "Daniel",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "9372", "5274560873703343",
              "12658309", 0, "5512739320", "Osorio", {}
          ),
          Cuenta(
              342455, "948", "Guillermo",
              time(nullptr), vector<double>({50, 50, 50, 50, 50, 50, 50}),
              "3789", "5075216458201382",
              "87906512", 2329, "5574481902", "Ruiz", {}
          ),
      }
  );
}