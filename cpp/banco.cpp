#include "banco.h"

#include <iostream>

// https://stackoverflow.com/questions/997512/string-representation-of-time-t
#include <ctime>
#include <fstream>

#define cansal "8059834059834082934820948359845834509384549423423454236573645654654623412557567464353528748237498237486472492018309127436423740238434"

Cuenta cargar_ususario(string & archivo) {
  fstream f;
  f.open(archivo);
  string nombre, super_key, nip, key, apellido, tel, tarjeta;
  vector<string> tarjetas_registradas = vector<string>();
  vector<double> gasto_semanal;
  double dinero, deuda;
  time_t fecha_vencimiento;
  
  getline(f, key);
  getline(f, super_key);
  getline(f, nip);
  getline(f, nombre);
  getline(f, apellido);
  getline(f, tel);
  getline(f, tarjeta);
  
  f >> fecha_vencimiento;
  f.ignore(numeric_limits<streamsize>::max(), '\n');
  
  f >> dinero;
  f.ignore(numeric_limits<streamsize>::max(), '\n');
  
  f >> deuda;
  f.ignore(numeric_limits<streamsize>::max(), '\n');
  
  string tarjeta_r;
  while(getline(f, tarjeta_r)) {
    tarjetas_registradas.push_back(move(tarjeta_r));
  }

  Cuenta cuenta = {
      key, /* 8 */
      super_key, /* 4 */
      nip, /* 3 */
      nombre,
      apellido,
      tel,
      fecha_vencimiento,
      tarjetas_registradas,
      gasto_semanal,
      dinero,
      deuda,
      tarjeta,
  };

  return cuenta;
}

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
  printf(
      "<h2 class='text-center'>Información de la Cuenta</h2>"
      "<h4>%s, %s:</h4>"
      "<p>Tarjeta: ",
      apellido.c_str(), nombre.c_str()
  );

  cout << tarjeta.substr(0, 4) << " ";
  cout << tarjeta.substr(4, 4) << " ";
  cout << tarjeta.substr(8, 4) << " ";
  cout << tarjeta.substr(12, 4);
  cout << " (" << nip << ")";
  cout << "</br>";
  // https://stackoverflow.com/questions/997512/string-representation-of-time-t
  std::tm *ptm = std::localtime(&fecha_vencimiento);
  char buffer[32];

  std::strftime(buffer, 32, "%m/%Y", ptm);

  cout << "    Teléfono: ";
  cout << tel.substr(0, 2) << " " << tel.substr(2, tel.length());
  cout << "</br>";
  cout << "    FV: " << buffer << "</br>";
  cout << "    Valid True" << "</p>";
  cout << "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>\n";
}


/// Método que verifica que una cadena sea igual a la contraseña registrada
/// \param contra El string, intento de contraseña a verificar
/// \return Devuelve true si es correcta
bool Cuenta::validarContra(const string &contra) const {
  return contra == key;
}

/// Muestra el saldo del usuario
void Cuenta::verSaldo() const {
  printf(
      "<h2 class='text-center'>Saldo de Cuenta</h2>"
      "<h4>%s, %s:</h4>"
      "<p>Saldo: %.2lf</br>Deuda: %.2lf</br>Total: %.2lf</p>"
      "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
      "\n",
      apellido.c_str(), nombre.c_str(),
      dinero, deuda,
      dinero - deuda
  );
}

/// Muestra las cuentas registradas para transferencias verificadas
void Cuenta::verCuentas() {
  if (tarjetas_registradas.empty()) {
    printf(
        "<h2 class='text-center'>Cuentas Registradas</h2>"
        "<p class='text-center'>No hay cuentas registradas!</br>Puedes registrar cuentas en Transferencias/Registrar cuentas</p>"
        "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
        "\n"
    );
  } else {
    printf(
        "<h2 class='text-center'>Cuentas Registradas</h2>"
        "<p>Cuentas registradas para transferencia:</p><p>"
    );
    for (int i = 0; i < tarjetas_registradas.size(); i++) {
      cout << i + 1 << ": "
           << tarjetas_registradas[i].substr(0, 4) << " "
           << tarjetas_registradas[i].substr(4, 4) << " "
           << tarjetas_registradas[i].substr(8, 4) << " "
           << tarjetas_registradas[i].substr(12, 4) << "</br>";
    }
    cout << "</p><div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>\n";
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
    printf(
        "<h2 class='text-center'>Error al Registrar Cuenta</h2>"
        "<p class='text-center'>Error: La cuenta ya está registrada</p>"
        "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
        "\n"
    );
  } else {
    tarjetas_registradas.push_back(t);
    printf(
        "<h2 class='text-center'>Éxito al Registrar Cuenta</h2>"
        "<p class='text-center'>Cuenta registrada con éxito!</p>"
        "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
        "\n"
    );
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

  if (0) {
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


bool contraEs_s(string &contra, Cuenta &cuenta) {
  return cuenta.validarContra(contra);
}

bool cuentaExiste_s(string &cuenta, Banco &banco) {
  ResB resultado = banco.buscarCuentaRaw(cuenta);

  return resultado.fue_exitosa;
}

bool cuentaExiste_o_exit_s(string &cuenta, Banco &banco) {
  bool es_exit = false;
  ResB resultado = banco.buscarCuentaRaw(cuenta);

  if (cuenta == cansal) {
    es_exit = true;
  }

  return resultado.fue_exitosa || es_exit;
}

bool esSuperKey_s(string &key, Cuenta &cuenta) {
  return cuenta.validarSuperContra(key);
}

bool esSuperKeyOCancela_s(string &key, Cuenta &cuenta) {
  return cuenta.validarSuperContra(key) || key == cansal;
}

bool cuentaExisteOCancela_s(string &cuenta, Banco &banco) {
  ResB resultado = banco.buscarCuentaRaw(cuenta);

  return resultado.fue_exitosa || cuenta == cansal;
}


vector<Cuenta> cuentas_iniciales() {
  vector<Cuenta> cuentas_default = vector<Cuenta>();
  
  string archivo = string("/Users/alejandro/pa-proyecto-tauri/cpp/test.txt"); 
  Cuenta cuenta = cargar_ususario(archivo);
  
  cuentas_default.push_back(cuenta);
  
  return cuentas_default;
}