#include "library.h"
#include <iostream>


int main() {
    vector<Cuenta> cuentas = vector<Cuenta>(
        {
            Cuenta(
                500, string("123"), string("Pepe"),
                time(nullptr), vector<double>({50, 50, 50, 50}),
                {3, 4, 4}, string("1234567890123456"),
                string("4324234")
            ),
            Cuenta(
                500, string("123"), string("Pepe"),
                time(nullptr), vector<double>({50, 50, 50, 50}),
                {3, 4, 4}, string("1234567890143456"),
                string("4324234")
            ),
            Cuenta(
                500, string("123"), string("Pepe"),
                time(nullptr), vector<double>({50, 50, 50, 50}),
                {3, 4, 4}, string("1234567890423456"),
                string("4324234")
            ),
            Cuenta(
                500, string("123"), string("Pepe"),
                time(nullptr), vector<double>({50, 50, 50, 50}),
                {3, 4, 4}, string("1234563890123456"),
                string("4324234")
            ),
            Cuenta(
                500, string("123"), string("Pepe"),
                time(nullptr), vector<double>({50, 50, 50, 50}),
                {3, 4, 4}, string("1234567844423456"),
                string("4324234")
            )
        }
    );
  Banco banco = Banco(cuentas);
  Cuenta * cuenta;
  int opcion;
  string tarjeta;

  std::cout << "Ingresa la tarjeta: ";
  cin >> tarjeta;
  //public: ResultadoB Banco::buscarCuentaRaw(const basic_string<char, char_traits<char>, allocator<char>> &cuenta)
  ResultadoB resultado = banco.buscarCuentaRaw(tarjeta);
  if (resultado.fue_exitosa)
  {
    cuenta = resultado.encontrada;
  } else {
    std::cout << "Tarjeta inválida\n";
    exit(0);
  }

  //
  string contra;
  cout << "Ingresa la contraseña: ";
  cin >> contra;

  if (!cuenta->validar_contra(contra)) {
    cout << "Contraseña invalida!\n";
    exit(0);
  }

#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif

  cout<<"Ingrese una opción: ";
  cin>>opcion;
  switch (opcion) {
    case 1:
      break;
    case 2:
      break;
    default:
      std::cout << "Opción valida\n";
  }

}
