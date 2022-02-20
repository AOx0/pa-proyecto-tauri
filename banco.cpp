#include "banco.h"

#include <iostream>

void Cuenta::agregarDinero(double cantidad) {
  dinero += cantidad;
}

double Cuenta::retirarDinero(double cantidad) {
  dinero -= cantidad;
  return cantidad;
}

void Cuenta::ver_cuenta() {
  cout<<"-------------------------------------"<<endl;
  cout<<"**** **** **** ";
  for (int i = 12; i < 16; ++i) {
    cout<<tarjeta[i];
  }

  printf("%18s\n", nip.c_str());
  printf("%21s", "");
  cout<<"Super Linea"<<endl;
  printf("%21s", "");
  cout<<"55 69636734 "<<endl;
  cout<<"Valid True"<<endl;
  cout<<fecha_vencimiento<<"         "<< nombre<<endl;
  cout<<"-------------------------------------"<<endl;
}

bool Cuenta::validar_contra(string contra) {
  if (contra == key)
  {
    cout<<"Contraseña valida"<<endl;
    return true;
  }
  return false;
}

Cuenta Banco::buscarCuenta() {
  string cuenta_str;
  ResultadoB cuenta;
  do {
    cout << "Ingresa un número de tarjeta: ";
    cin >> cuenta_str;
    cuenta = buscarCuentaRaw(cuenta_str);

    if (cuenta.fue_exitosa)
    {
      return *cuenta.encontrada;
    } else
    {
      cout << "Cuenta no encontrada\n";
    }
  } while (!cuenta.fue_exitosa);

  return *cuenta.encontrada;
}

ResultadoB Banco::buscarCuentaRaw(const string& tarjeta)
{
  ResultadoB resultado = ResultadoB();
  for (auto &cuenta: cuentas)
  {
      if (tarjeta == cuenta.tarjeta)
      {
        resultado.fue_exitosa = true;
        resultado.encontrada = &cuenta;
        return resultado;
      }
  }
  return resultado;
}


