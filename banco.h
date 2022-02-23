#ifndef PROYECTO_LIBRARY_H
#define PROYECTO_LIBRARY_H

#include <iostream>
#include <vector>

using namespace std;

struct Cuenta {
private:
  string key; /* 8 */
  string super_key; /* 4 */
  string nip; /* 3 */
  string nombre;
  time_t fecha_vencimiento;
  vector<double> gasto_semanal;
  double dinero;
  double deuda;
public:
  string tarjeta;

  Cuenta(double dinero, string nip, string nombre, time_t fecha_vencimiento, vector<double> gasto_semanal,
         string super_key, string tarjeta, string key, double deuda)
      : dinero(dinero), nip(move(nip)), nombre(move(nombre)), fecha_vencimiento(fecha_vencimiento),
        gasto_semanal(move(gasto_semanal)), super_key(move(super_key)), tarjeta(move(tarjeta)), key(move(key)),
        deuda(deuda) {};

  void agregarDinero(double cantidad);

  double retirarDinero(double cantidad);

  void ver_cuenta();

  bool validar_contra(string contra);
};

struct ResultadoB {
public:
  Cuenta *encontrada;
  bool fue_exitosa;

  ResultadoB() : fue_exitosa(false), encontrada(nullptr) {};
};

struct Banco {
private:
  vector<Cuenta> cuentas;
  int contra = 12344321;
public:
  explicit Banco(vector<Cuenta> cuentas) : cuentas(move(cuentas)) {};

  ResultadoB buscarCuentaRaw(const string &cuenta);

  //vector<Cuenta> generarCuentas();
  Cuenta buscarCuenta();
};

// Funciones con instancia por argumento
bool contraEs_s(string &contra, Cuenta cuenta);

bool cuentaExiste_s(string &cuenta, Banco banco);


vector<Cuenta> cuentas_iniciales();

#endif //PROYECTO_LIBRARY_H
