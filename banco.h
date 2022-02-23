#ifndef PROYECTO_LIBRARY_H
#define PROYECTO_LIBRARY_H

#include <iostream>
#include <utility>
#include <vector>

using namespace std;

struct Cuenta {
public:
  string key; /* 8 */
  string super_key; /* 4 */
  string nip; /* 3 */
  string nombre;
  string apellido;
  string tel;
  time_t fecha_vencimiento;
  vector<string> tarjetas_registradas;
  vector<double> gasto_semanal;
  double dinero;
  double deuda;
  string tarjeta;

  Cuenta(double dinero, string nip, string nombre, time_t fecha_vencimiento, vector<double> gasto_semanal,
         string super_key, string tarjeta, string key, double deuda, string tel, string apellido,
         vector<string> tarjetas_registradas)
      : dinero(dinero), nip(move(nip)), nombre(move(nombre)), fecha_vencimiento(fecha_vencimiento),
        gasto_semanal(move(gasto_semanal)), super_key(move(super_key)), tarjeta(move(tarjeta)), key(move(key)),
        deuda(deuda), tel(move(tel)), apellido(move(apellido)), tarjetas_registradas(move(tarjetas_registradas)) {};

  void agregarDinero(double cantidad);

  void verSaldo();

  void verCuentas();

  double retirarDinero(double cantidad);

  void ver_cuenta();

  bool validar_contra(const string &contra);

  bool validar_super_contra(const string &contra);

  bool cuentaYaRegistrada_t(const string &t);

  void registrarCuenta_t(const string &t);

  void eliminarCuenta_t(const string &t);

  void transferir(Cuenta &target, double cantidad) {
    dinero -= cantidad;
    target.dinero += cantidad;

    if (1) {
      cout << "    Tu dinero: " << dinero + cantidad << "->" << dinero << "\n";
      cout << "    Su dinero: " << target.dinero - cantidad << "->" << target.dinero << "\n";
    }
  }
};

struct ResB {
public:
  Cuenta *encontrada;
  bool fue_exitosa;

  ResB() : fue_exitosa(false), encontrada(nullptr) {};
};

struct Banco {
private:
  vector<Cuenta> cuentas;
  int contra = 12344321;
public:
  explicit Banco(vector<Cuenta> cuentas) : cuentas(move(cuentas)) {};

  ResB buscarCuentaRaw(const string &cuenta);

  //vector<Cuenta> generarCuentas();
  Cuenta buscarCuenta();
};

// Funciones con instancia por argumento
bool contraEs_s(string &contra, Cuenta cuenta);

bool cuentaExiste_s(string &cuenta, Banco banco);

bool cuentaExisteOCancela_s(string &cuenta, Banco banco);

bool esSuperKey_s(string &key, Cuenta cuenta);

vector<Cuenta> cuentas_iniciales();

#endif //PROYECTO_LIBRARY_H
