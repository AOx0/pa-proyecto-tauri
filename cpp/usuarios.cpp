#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

#include "com.h"
#include "banco.h"
#include "utils.h"

#define SALIR 50
#define CANCEL_OR_EXIT "8059834059834082934820948359845834509384549423423454236573645654654623412557567464353528748237498237486472492018309127436423740238434"

/*
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

bool pedirSuperKey(Cuenta &cuenta, const string& mensaje =  std::string(""));
 */

vector<CuentaRaw> pedir_index(Communicator & c);

void load_main(Communicator &c, const Cuenta &cuenta, string &nul, stringstream &sout, stringstream &sin);

int main(int argc, char *argv[]) {
  {

    string nul;
    double basura;

    stringstream sout; //Es como un cout pero no recibe valores de la consola
    stringstream sin; //Lo envian al archuivo cifrado

    Communicator c = Communicator(argv[1]);

    ofstream ou;
    ou.open(c.log_file);

    string files_names_raw, name, path, sub;
    vector<CuentaRaw> cuentas = vector<CuentaRaw>();
    
    c.receive(&sin);
    getline(sin, files_names_raw);

    ou << "Archivos: " << files_names_raw << endl;

    istringstream in_names(files_names_raw);
    while(getline(in_names, sub, ';')){
      istringstream in_data(sub);

      getline(in_data, name, '*');
      getline(in_data, path, '*');

      CuentaRaw cuenta_raw = { name, path };
      cuentas.push_back(cuenta_raw);
      ou << cuenta_raw.cuenta << endl;
    }

    Banco banco = Banco(cuentas);
    Cuenta cuenta;

    string estado;
    while (true)
    {
      /* code */
    
      c.receive(&sin);
      sin>>nul;

      if (nul == "Change_To_Login") {
        sout << "Changed to login!";
        c.send(&sout);
      }

      if (nul == "login") {
        sout << "Login in";
        c.send(&sout);

        string tarjeta = pedirValorX(c, NUMBERS, true);

        cout << 1 << endl;

        string contra = pedirValorX(c, {}, false);

        if (!cuentaExiste_o_exit_s(tarjeta, banco))
        {
          sout << "Error: No existe la cuenta\n";
        }
        else
        {
          cuenta = banco.buscarCuentaRaw(tarjeta).encontrada;
          if (!contraEs_s(contra, cuenta))
          {
            sout << "Error: Contraseña inválida\n";
          } else {
            sout << "change to main" << endl;
          }
        }
        c.send(&sout);
      }

      if (nul == "Change_To_Main")
      {
        sout << "Changed to main\n";
        c.send(&sout);
        load_main(c, cuenta, nul, sout, sin);

        c.receive(&sin);

        sout << "<canvas\n data-bss-chart='{\"type\":\"line\",\"data\":{\"labels\":[\".\",\".\",\".\",\".\",\".\",\".\",\".\",\"Now\"],\"datasets\":[{\"label\":\"\",\"fill\":true,\"data\":";
        sout << "[\""<< cuenta.gasto_semanal[0] <<"\",\"" << cuenta.gasto_semanal[1] << "\",\""<< cuenta.gasto_semanal[2] <<"\",\""<< cuenta.gasto_semanal[3] <<"\",\""<< cuenta.gasto_semanal[4] <<"\",\""<< cuenta.gasto_semanal[5] <<"\",\""<< cuenta.gasto_semanal[6] <<"\",\""<< cuenta.gasto_semanal[7] <<"\"]";
        sout << ",\"backgroundColor\":\"rgba(78, 115, 223, 0.05)\",\"borderColor\":\"rgba(78, 115, 223, 1)\"}]},\"options\":{\"maintainAspectRatio\":false,\"legend\":{\"display\":false,\"labels\":{\"fontStyle\":\"normal\"}},\"title\":{\"fontStyle\":\"normal\"},\"scales\":{\"xAxes\":[{\"gridLines\":{\"color\":\"rgb(234, 236, 244)\",\"zeroLineColor\":\"rgb(234, 236, 244)\",\"drawBorder\":false,\"drawTicks\":false,\"borderDash\":[\"2\"],\"zeroLineBorderDash\":[\"2\"],\"drawOnChartArea\":false},\"ticks\":{\"fontColor\":\"#858796\",\"fontStyle\":\"normal\",\"padding\":20}}],\"yAxes\":[{\"gridLines\":{\"color\":\"rgb(234, 236, 244)\",\"zeroLineColor\":\"rgb(234, 236, 244)\",\"drawBorder\":false,\"drawTicks\":false,\"borderDash\":[\"2\"],\"zeroLineBorderDash\":[\"2\"]},\"ticks\":{\"fontColor\":\"#858796\",\"fontStyle\":\"normal\",\"padding\":20}}]}}}";
        sout << "'\nstyle=\"display: block; width: 1029px; height: 320px;\"\nclass=\"chartjs-render-monitor\" width=\"1029\" height=\"320\"></canvas>";
        c.send(&sout);

        estado = "main";
      }

      if (nul == "Change_To_Deudas")
      {
        sout << "Changed to deuda\n";
        c.send(&sout);
        load_main(c, cuenta, nul, sout, sin);
        estado = "deudas";
      }

      if (estado == "table" && nul == "agregar_cuenta") {
        sout << "Agregar cuenta\n";
        c.send(&sout);

        c.receive(&sin);
        sin >> nul;

        sout << "Recibido crack";
        c.send(&sout);

        string pass;
        c.receive(&sin);
        sin >> pass;

        bool existe = banco.buscarCuentaRaw(nul).fue_exitosa;
        bool acceso = cuenta.validarSuperContra(pass);

        if (!acceso || !existe || cuenta.tarjeta == nul) {
          sout << "Error: ";
          if (cuenta.tarjeta == nul) {
            sout << "No puedes registrarte a ti mismo";
          } else {
            if (!existe) sout << "No existe la cuenta";
            if (!existe && !acceso) sout << " & ";
            if (!acceso) sout << "La contraseña no es correcta";
          }
          
          c.send(&sout);
        } else {
          sout << "Agregando cuenta";
          c.send(&sout);

          c.receive(&sin);
          if (acceso && existe) {
            if (cuenta.cuentaYaRegistrada_t(nul)) {
              sout << "Error: No puedes agregar una cuenta ya registrada";
            } else {
              cuenta.registrarCuenta_t(nul);
              cuenta.guardar_usuario();
              sout << "Agregada";
            }
            c.send(&sout);
          }
        }
        
      }

      if (estado == "table" && nul == "pre_transferir_a_cuenta") {
        sout << cuenta.dinero;
        c.send(&sout);

        c.receive(&sin);
        sin >> nul;

        double cantidad_a_transferir;
        sout << (cuenta.cuentaYaRegistrada_t(nul) ? "true" : "false");
        c.send(&sout);

        c.receive(&sin);
        sin >> cantidad_a_transferir;

        sout << cuenta.tarjeta;
        c.send(&sout);

        string decision;
        c.receive(&sin);
        sin >> decision;

        if (decision == "transfiere_ya") {
          Cuenta a_transferir = banco.buscarCuentaRaw(nul).encontrada;
          cuenta.agregar_a_historial( cuenta.dinero - cantidad_a_transferir);
          a_transferir.agregar_a_historial( a_transferir.dinero - cantidad_a_transferir);
          cuenta.transferir(a_transferir, cantidad_a_transferir);

          sout << "Transferencia hecha";
        } else {
          sout << "Transferencia no hecha";
        }

        c.send(&sout);
      }

      if (estado == "table" && nul == "eliminar_cuenta") {
        sout << "Eliminar cuenta\n";
        c.send(&sout);

        ou << "Pidiendo cuenta a eliminar..." << endl;


        c.receive(&sin); // Pedimos la cuenta
        sin >> nul;

        ou << "Cuenta a eliminar: " << nul << endl;

        bool success = cuenta.eliminarCuenta_t(nul);

        if (success) {
          sout << "Cuenta eliminada\n";
        } else {
          sout << "No se pudo eliminar la cuenta\n";
        }

        cuenta.guardar_usuario();
        
        c.send(&sout);
      }

      if (estado == "profile" && nul == "update_data") {
        sout << "Update data";
        c.send(&sout);

        string nombre, apellido, num, pass;
        c.receive(&sin);
        sin >> nombre;

        sout << " " << endl;
        c.send(&sout);

        c.receive(&sin);
        sin >> apellido;

        sout << " " << endl;
        c.send(&sout);


        c.receive(&sin);
        sin >> num;

        sout << " " << endl;
        c.send(&sout);


        c.receive(&sin);
        sin >> pass;

        if (pass == cuenta.key) {
          cuenta.nombre = nombre;
          cuenta.apellido = apellido;
          cuenta.tel = num;
          cuenta.guardar_usuario();
          sout << "Actualizando datos";
        } else {
          sout << "Error: Contraseña incorrecta";
        }
        c.send(&sout);
      }

      if (estado == "deudas" && nul == "solicitar_deuda") {
        sout << "Solicitar deuda";
        c.send(&sout);

        double cantidad;
        c.receive(&sin);
        sin >> cantidad;

        if (cantidad/cuenta.dinero > 0.7) {
          sout << "Error: No se puede solicitar más del 70%% de la cantidad disponible de saldo" ;
        } else if (cuenta.deuda/cuenta.dinero > 0.15) {
            sout << "Error: No puedes solicitar préstamos si tienes ya una gran cantidad de deuda por pagar" ;
        } else {
          cuenta.agregar_a_historial( cuenta.dinero + cantidad);
          cuenta.deuda += cantidad;
          cuenta.dinero += cantidad;

          sout << "Éxito! Préstamo solicitado con éxito.";

          cuenta.guardar_usuario();
        }
        c.send(&sout);

      }

      if (estado == "deudas" && nul == "pagar_deuda") {
        sout << "Pagar deuda";
        c.send(&sout);

        double cantidad;
        c.receive(&sin);
        sin >> cantidad;


        if (cantidad > cuenta.dinero) {
          sout << "Error: No cuentas con los fondos suficientes para pagar la deuda";
        } else if (cuenta.deuda == 0) {
          sout << "Error: No cuentas con una deuda por pagar";
        } else if (cuenta.deuda - cantidad < 0) {
          sout << "Error: No puedes pagar una cantidad mayor a la deuda";
        } else {

          cuenta.agregar_a_historial( cuenta.dinero - cantidad);
          cuenta.retirarDinero(cantidad);
          cuenta.deuda -= cantidad;
          sout << "Éxito al pagar la deuda";

          cuenta.guardar_usuario();
        }
        c.send(&sout);

      }

      if (nul == "Change_To_Table") {
        sout << "Changed to table\n";
        c.send(&sout);

        estado = "table";

        c.receive(&sin);

        for (auto & t: cuenta.tarjetas_registradas) {
          Cuenta cuenta_registrada = banco.buscarCuentaRaw(t).encontrada;
          sout << "<tr><td>" << cuenta_registrada.nombre << " " << cuenta_registrada.apellido << "</td><td>" << cuenta_registrada.tarjeta << "</td>"
          << "<td><button id='botón_agregar_cuenta' class='btn btn-primary btn-sm' onclick=\"document.getElementById('tarjeta_transferir').value = '" << cuenta_registrada.tarjeta << "'\">Transferir</button>&#x09;&#x09;"
          << "<button id='botón_agregar_cuenta' class='btn btn-primary btn-sm' onclick=\"eliminar_cuenta('" << cuenta_registrada.tarjeta << "')\">Eliminar</button></td></tr>"
          << endl;
        }

        if (cuenta.tarjetas_registradas.size() == 0) {
          sout << "<tr><td>No hay cuentas registradas</td><td></td></tr>\n";
        } else {
          sout << endl;
        }



        c.send(&sout);
      }
        if (nul == "Change_To_profile")
        {
            sout << "Changed to profile\n";
            c.send(&sout);
            c.receive(&sin);
            estado = "profile";

            sout<<cuenta.nombre<<" "<<cuenta.apellido;
            c.send(&sout);
            c.receive(&sin);

            sout<<cuenta.nombre;
            c.send(&sout);
            c.receive(&sin);

            sout<<cuenta.apellido;
            c.send(&sout);
            c.receive(&sin);

            sout<<cuenta.tel;
            c.send(&sout);


        }
      //Terminar prueba
      //Salir del dashboard
        else if (nul == CANCEL_OR_EXIT)
        {
            sout << "EXIT";
            c.send(&sout);
            break;
          }
      //termina el bucle
    }
  }
}

void load_main(Communicator &c, const Cuenta &cuenta, string &nul, stringstream &sout, stringstream &sin)
{
  string nombre = cuenta.nombre, apellido = cuenta.apellido, tarjetas = cuenta.tarjeta;
  double dinero = cuenta.dinero, deuda = cuenta.deuda;

  c.receive(&sin);

  sout<<nombre<<" "<<apellido<<endl;
  c.send(&sout);

  c.receive(&sin);

  sout<<dinero<<endl;
  c.send(&sout);

  c.receive(&sin);

  sout<<tarjetas<<endl;
  c.send(&sout);

  c.receive(&sin);

  sout<<deuda<<endl;
  c.send(&sout);
}

/*
int menuPrincipal(Cuenta &cuenta, Banco &banco) {
  printf(
      "<h2 class='text-center'>Súper Linea</h2></br>"
      "<p class='text-center'>Submenus</br>Opciones con el símbolo * pueden requerir su Super-key</p>"
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

  int option = pedirValor(1, SALIR), val;

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
  int option;
  string cont;

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

  option = pedirValor(1, SALIR);

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
    default: break;
  }

  cin >> cont;
  return option;
}

int menuTransferencias(Cuenta &cuenta, Banco &banco) {

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

  int option = pedirValor(1, SALIR);
  string cont;


  if (option < 4 && option > 1) {
    if (pedirSuperKey(cuenta)) return option;
  }


  bool cancelado = false;
  switch (option) {
    case 1: {
      string a_transferir;
      int valido = 0;

      do {
        if (valido == 0)
          printf(
              "<h2 class='text-center'>Cuenta a Transferir</h2>"
              "<p class='text-center'>Ingresa la tarjeta a transferir: </p>"
              "\n"
          );
        else
          printf(
              "<h2 class='text-center'>Cuenta a Transferir</h2>"
              "<p class='text-center'>Tarjeta inválida. No puedes transferir dinero a ti mismo!</br>Ingresa la tarjeta a transferir: </p>"
              "\n"
          );

        a_transferir = pedirValor<Banco>(
            banco,
            &cuentaExisteOCancela_s, "<h2 class='text-center'>Cuenta a Transferir</h2><p class='text-center'>Tarjeta inválida.</br>Ingresa la tarjeta a transferir: </p><div class='text-center' class='mb-3'>", "Cuenta a Transferir", NUMBERS, true
        );

        if (a_transferir == CANCEL_OR_EXIT) {
          cancelado = true;
          break;
        }

        if (cuenta.tarjeta == a_transferir) {
          valido = 2;
        } else {
          valido = 1;
        }
      } while (valido != 1);

      if (cancelado) {
        return option;
      }

      printf(
          "<h2 class='text-center'>Cantidad a Transferir</h2>"
          "<p class='text-center'>Ingresa la cantidad a transferir:</p>"
          "\n"
      );

      double cantidad = pedirValor(100.0, 5000000.0, {','}, "Cantidad a Transferir", NUMBERS, true);

      if (!count(cuenta.tarjetas_registradas.begin(), cuenta.tarjetas_registradas.end(), a_transferir)) {
        if (pedirSuperKey(cuenta)) return option;
      }

      cout << "FILES_PLS" << endl;
      banco = Banco(pedir_index());
      Cuenta target =  banco.buscarCuentaRaw(a_transferir).encontrada;

      if (cantidad > cuenta.dinero) {
        printf(
            "<h2 class='text-center'>Error al Transferir</h2>"
            "<p class='text-center'>No hay suficientes fondos para realizar la transferencia</p>"
            "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
            "\n"
        );
      } else {

        cuenta.transferir(target, cantidad);
        double restante = cuenta.dinero;
        cout << "<h2 class='text-center'>Éxito al Transferir</h2>";
        cout << "<p>Éxito. Se transfirieron $" << cantidad << " MXN</br>";
        cout << "Saldo restante: $" << restante << ".00</p>";
        cout << "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>\n";
      }

    }
      break;
    case 2: {
      string a_agregar;
      int valido = 0;
      do {
        cout << "FILES_PLS" << endl;
        banco = Banco(pedir_index());

        if (valido == 0)
          printf(
              "<h2 class='text-center'>Registrar Cuenta</h2>"
              "<p class='text-center'>Al registrar puedes transferir a dicha cuenta sin usar Super-Key.</br>Ingresa la tarjeta a registrar</p>"
              "\n"
          );
        else
          printf(
              "<h2 class='text-center'>Registrar Cuenta</h2>"
              "<p class='text-center'>Tarjeta inválida. No puedes registrarte a ti mismo!</br>Ingresa la tarjeta a registrar</p>"
              "\n"
          );

        a_agregar = pedirValor<Banco>(
            banco,
            &cuentaExisteOCancela_s, "<h2 class='text-center'>Registrar Cuenta</h2><p class='text-center'>Tarjeta inválida. No puedes registrarte a ti mismo!</br>Ingresa la tarjeta a registrar</p>" , "Registrar Cuenta", NUMBERS, true
        );

        if (a_agregar == CANCEL_OR_EXIT) {
          return option;
        }

        if (cuenta.tarjeta == a_agregar) {
          valido = 2;
        } else {
          valido = 1;
        }
      } while (valido != 1);

      cuenta.registrarCuenta_t(a_agregar);
      cuenta.guardar_usuario();
    }
      break;
    case 3: {
      if (cuenta.tarjetas_registradas.empty()) {
        printf(
            "<h2 class='text-center'>Eliminar Cuenta</h2>"
            "<p class='text-center'>No hay cuentas registradas.</p>"
            "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
            "\n"
        );
        cin >> cont;
        return option;
      }


      string a_borrar;
      int valido = 0;
      do {
        if (valido == 0)
        printf(
            "<h2 class='text-center'>Eliminar Cuenta</h2>"
            "<p class='text-center'>Ingresa la tarjeta a eliminar</p>"
            "\n"
        );
        else if (valido == -1)
          printf(
              "<h2 class='text-center'>Error al eliminar Cuenta</h2>"
              "<p class='text-center'>Tarjeta inválida. No está registrada como tarjeta de transferencia verificada.</p>"
              "\n"
          );
        else if (valido == -2)
          printf(
              "<h2 class='text-center'>Error al eliminar Cuenta</h2>"
              "<p class='text-center'>Tarjeta inválida. No puedes registrarte a ti mismo!</p>"
              "\n"
          );

        a_borrar = pedirValor<Banco>(
            banco,
            &cuentaExisteOCancela_s, "<h2 class='text-center'>Eliminar Cuenta</h2><p class='text-center'>Tarjeta Inválida</br>Ingresa la tarjeta a eliminar</p>", "Eliminar Cuenta", NUMBERS, true
        );

        if (a_borrar == CANCEL_OR_EXIT) {
          return option;
        } else {
          if (!std::count(cuenta.tarjetas_registradas.begin(), cuenta.tarjetas_registradas.end(), a_borrar)) {
            valido = -1;
          } else if (cuenta.tarjeta == a_borrar) {
            valido = -2;
          } else {
            valido = 1;
          }
        }
      } while (valido != 1);

      cuenta.eliminarCuenta_t(a_borrar);
      printf(
          "<h2 class='text-center'>Éxito al eliminar Cuenta</h2>"
          "<p class='text-center'>Cuenta eliminada con éxito!</p>"
          "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
          "\n"
      );
      cuenta.guardar_usuario();
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
    default: break;
  }

  cin >> cont;
  return option;
}

int menuDepRet(Cuenta &cuenta, Banco &banco) {

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

  int option = pedirValor(1, SALIR);
  string cont;


  double cantidad;


  switch (option) {
    case 1:
      cuenta.verSaldo();
      break;
    case 2:
        printf(
          "<h2 class='text-center'>Depósito</h2>"
          "<p class='text-center'>Ingresa la cantidad a depositar</p>"
          "\n"
        );
      cantidad = pedirValor(100, 50000, {','}, "Depósito", NUMBERS, true);
      cuenta.agregarDinero(cantidad);
      continuar();
      cout 
        << "<h2 class='text-center'>Depósito</h2>"
        << "<p class='text-center'>Éxito. Se depositaron: $" << cantidad << " MXN</p>"
        << "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
        << "\n"
      ;
      cuenta.guardar_usuario();
      break;
    case 3:
      printf(
        "<h2 class='text-center'>Retiro</h2>"
        "<p class='text-center'>Ingresa la cantidad a retirar</p>"
        "\n"
      );
      cantidad = pedirValor(100, 50000, {','}, "Retiro", NUMBERS, true);

      if (cantidad > 2500) 
        if (pedirSuperKey(cuenta, "<p class='text-center'>Una cantidad mayor de 2500 requiere Super-Key</p>")) return option;

      if (cantidad > cuenta.dinero) {
        printf(
          "<h2 class='text-center'>Error al retirar</h2>"
          "<p class='text-center'>Error: No hay suficientes fondos</p>"
          "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
          "\n"
        );
      } else {
        double restante = cuenta.retirarDinero(cantidad);
        cout
          << "<h2 class='text-center'>Éxito al retirar</h2>"
          << "<p class='text-center'>Éxito. Se retiraron $" << cantidad << " MXN</br>"
          << "Saldo restante: $" << restante << ".00</br>"
          << "Recoge el dinero de la bandeja.</p>"
          << "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
          << "\n"
        ;

        cuenta.guardar_usuario();
      }
      break;
    case 4:
      return option;
    case 5:
      return SALIR;
    default: break;
  }

  cin >> cont;
  return option;
}

int menuPrestamos(Cuenta &cuenta, Banco &banco) {

  printf(
      "<h2 class='text-center'>Menu Prestamos</h2>"
      "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Pagar deuda</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no2' class='btn btn-primary'>Solicitar préstamo (*)</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no3' class='btn btn-primary'>Ver saldo</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='back' class='btn btn-primary'>Regresar</button></div>"
      "</br>"
      "<div class='text-center' class='mb-3'><button id='no5' class='btn btn-primary'>Salir</button></div>"
      "</p>\n"
  );

  int option = pedirValor(1, SALIR);
  string cont;



  switch (option) {
    case 1: {
      if (cuenta.deuda == 0) {
        printf(
          "<h2 class='text-center'>Deudas</h2>"
          "<p class='text-center'>No cuentas con una deuda que pagar</p>"
          "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
          "\n"
        );
        break;
      }

      if (pedirSuperKey(cuenta)) return option;

      cout
        << "<h2 class='text-center'>Deudas</h2>"
        << "<p class='text-center'>Deuda actual: $" << cuenta.deuda << "</br>"
        << "Ingresa la cantidad a pagar</p>"
        << "\n"
      ;

      double cantidad = pedirValor(1.0, cuenta.deuda, {','}, "AAAA", NUMBERS, true);

      if (cantidad > cuenta.dinero) {
        printf(
          "<h2 class='text-center'>Sin saldo</h2>"
          "<p class='text-center'>No cuentas con los fondos suficientes para pagar la deuda</p>"
          "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
          "\n"
        );
      } else {

        cuenta.retirarDinero(cantidad);
        cuenta.deuda -= cantidad;
        cout
          << "<h2 class='text-center'>Éxito</h2>"
          << "<p class='text-center'>Resultado de la operación:</br>"
          << "Deuda: " << cuenta.deuda + cantidad << " -> " << cuenta.deuda << "</br>"
          << "Saldo: " << cuenta.dinero + cantidad << " -> " << cuenta.dinero << "</br>"
          << "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
          << "\n"
        ;

        cuenta.guardar_usuario();
      }

    }
      break;
    case 2: {

      if (pedirSuperKey(cuenta)) return option;

      cout
          << "<h2 class='text-center'>Solicitud Préstamo</h2>"
          << "<p class='text-center'>Ingresa la cantidad que desea solicitar:</br>"
          << "Saldo actual: $" << cuenta.dinero << "</br>"
          << "Deuda actual: $" << cuenta.deuda << "</p>"
          << "\n"
        ;

      double cantidad = pedirValor(1.0, cuenta.dinero * 0.7, {','}, "Solicitud Préstamo", NUMBERS, true);

      if (cantidad/cuenta.dinero > 0.7) {
        printf(
          "<h2 class='text-center'>Préstamo muy alto</h2>"
          "<p class='text-center'>No se puede solicitar más del 70%% de la cantidad disponible de saldo</p>"
          "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
          "\n"
        );
      } else if (cuenta.deuda/cuenta.dinero > 0.15) {
        printf(
          "<h2 class='text-center'>Pago de préstamo pendiente</h2>"
          "<p class='text-center'>No puedes solicitar préstamos si tienes ya una gran cantidad de deuda por pagar</p>"
          "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
          "\n"
        );
      } else {
        cuenta.deuda += cantidad;
        cuenta.dinero += cantidad;

        cout
          << "<h2 class='text-center'>Éxito</h2>"
          << "<p class='text-center'>Préstamo solicitado con éxito. Resultado de la operación:</br>"
          << "Deuda: " << cuenta.deuda - cantidad << " -> " << cuenta.deuda << "</br>"
          << "Saldo: " << cuenta.dinero - cantidad << " -> " << cuenta.dinero << "</br>"
          << "<div class='text-center' class='mb-3'><button id='no1' class='btn btn-primary'>Regresar</button></div>"
          << "\n"
        ;

        cuenta.guardar_usuario();
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
    default: break;
  }

  cin >> cont;
  return option;
}

bool pedirSuperKey(Cuenta &cuenta, const string& mensaje) {
  printf(
      "<h2 class='text-center'>Super Key</h2>"
      "%s"
      "<p class='text-center'>La operación requiere Super Key. Ingrésala: </p>"
      "\n", mensaje.c_str()
  );

  string key = pedirValor<Cuenta>(
      cuenta,
      &esSuperKeyOCancela_s,  "<h2 class='text-center'>Super Key</h2><p class='text-center'>Super Key Inválida. Ingresa tu Super Key: </p>", "Super Key"
  );

  return key == CANCEL_OR_EXIT;
}

*/