#ifndef PROYECTO_LIBRARY_H
#define PROYECTO_LIBRARY_H

#include <iostream>
#include <vector>

using namespace std;

/// Estructura que representa la Cuenta de un usuario.
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

  /// El constructor de la estructura Cuenta
  Cuenta(double dinero, string nip, string nombre, time_t fecha_vencimiento, vector<double> gasto_semanal,
         string super_key, string tarjeta, string key, double deuda, string tel, string apellido,
         vector<string> tarjetas_registradas)
      : dinero(dinero), nip(move(nip)), nombre(move(nombre)), fecha_vencimiento(fecha_vencimiento),
        gasto_semanal(move(gasto_semanal)), super_key(move(super_key)), tarjeta(move(tarjeta)), key(move(key)),
        deuda(deuda), tel(move(tel)), apellido(move(apellido)), tarjetas_registradas(move(tarjetas_registradas)) {};

  /// Suma al saldo de sí mismo la cantidad ingresada
  /// \param cantidad La cantidad a agregar
  void agregarDinero(double cantidad);

  /// Muestra el saldo del usuario
  void verSaldo() const;

  /// Muestra las cuentas registradas para transferencias verificadas
  void verCuentas();

  /// Disminuye la cantidad al saldo y devuelve el saldo restante
  /// \param cantidad La cantidad a restar
  /// \return El valor del saldo restante
  double retirarDinero(double cantidad);

  /// Muestra la información del usuario
  void verCuenta();

  /// Método que verifica que una cadena sea igual a la contraseña registrada
  /// \param contra El string, intento de contraseña a verificar
  /// \return Devuelve true si es correcta
  bool validarContra(const string &contra) const;

  /// Método que verifica que una cadena sea igual a la super-contraseña registrada
  /// \param contra El string, intento de contraseña a verificar
  /// \return Devuelve true si es correcta
  bool validarSuperContra(const string &contra) const;

  /// Verifica si una tarjeta dada, en forma de string, está ya registrada como cuenta de transferencia verificada
  /// \param t La tarjeta que se desea conocer si se encuentra ya registrada
  /// \return true si ya se encuentra registrada
  bool cuentaYaRegistrada_t(const string &t);

  /// Registra t como cuenta de transferencia verificada
  /// \param t La tarjeta que se desea registrar
  void registrarCuenta_t(const string &t);

  /// Elimina t del vector de tarjetas registradas
  /// \param t La tarjeta que se desea registrar
  void eliminarCuenta_t(const string &t);

  void transferir(Cuenta &target, double cantidad);
};

struct ResB {
public:
  Cuenta *encontrada;
  bool fue_exitosa;

  ResB() : fue_exitosa(false), encontrada(nullptr) {};
};

/// Estructura que representa al banco, que contiene un conjunto de cuentas
struct Banco {
private:
  vector<Cuenta> cuentas;
  int contra = 12344321;
public:
  explicit Banco(vector<Cuenta> cuentas) : cuentas(move(cuentas)) {};

  /// Método que busca dentro de las cuentas del banco la cuenta dada (tarjeta)
  /// \param cuenta El srting con el número de cuenta (tarjeta) a buscar
  /// \return Un resultado que contiene el estatus de la búsqueda y en caso de ser exitosa un apuntador a la instancia de Cuenta
  ResB buscarCuentaRaw(const string &cuenta);
};

// Funciones con el propósito de verificar.

bool contraEs_s(string &contra, Cuenta &cuenta);

bool cuentaExiste_s(string &cuenta, Banco &banco);

bool cuentaExiste_o_exit_s(string &cuenta, Banco &banco);

bool cuentaExisteOCancela_s(string &cuenta, Banco &banco);

bool esSuperKey_s(string &key, Cuenta &cuenta);

bool esSuperKeyOCancela_s(string &key, Cuenta &cuenta);

vector<Cuenta> cuentas_iniciales();

#endif //PROYECTO_LIBRARY_H
