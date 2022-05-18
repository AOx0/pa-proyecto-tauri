//
// Created by Alejandro D on 15/05/22.
//

#ifndef USUARIOS_COM_H
#define USUARIOS_COM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct DynArray {
  uint8_t* array;
  size_t length;
};

extern "C" DynArray cypher_bytes(uint8_t* n, uint32_t l);

extern "C" DynArray decipher_bytes(uint8_t* n, uint32_t l);

class Communicator {
public:
  string file;
  string log_file;
  explicit Communicator(string file) : file(move(file)), log_file(file + ".log") {
    ofstream f;
    f.open("/Users/alejandro/pa-proyecto-tauri/cpp/l", ios::app);
    f << this->file << endl;
    f << this->log_file << endl;
    f.close();
  
  }

  void send(stringstream * msg) {
    ofstream f;
    f.open(log_file, ios::app);
    string message = msg->str();

    f << "Enviando: " << message << endl;

    f.close();

    std::ofstream ofs;
    ofs.open(file, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary );

    vector<uint8_t> bytes;
    for (auto c: message) {
      bytes.push_back((uint8_t)c);
    }


    f << "Cifrando mensaje" << endl;
    DynArray cifrado = cypher_bytes(&(bytes[0]), bytes.size());

    vector<char> to_write = vector<char>();

    for (int i=0; i<cifrado.length; i++) {
      to_write.push_back((char)cifrado.array[i]);
    }

    ofs.write(&(to_write[0]), to_write.size());

    ofs.close();

    msg->str(std::string()); 

    cout << "END SEND" << endl;
  }

  void receive(stringstream * msg) {
    msg->str(std::string()); 
    string signal;

     ofstream f;
    f.open(log_file, ios::app );
    
    getline(cin, signal);

    f << "Recibido notificación: " << signal << endl;

    std::ifstream ifs;
    ifs.open(file, ifstream::binary);

    vector<uint8_t> bytes;

    vector<char> data = vector<char>(istreambuf_iterator<char>(ifs),(istreambuf_iterator<char>()));
    char* charArray = &data[0];
    size_t arraySize = data.size();

    for (int i=0; i < arraySize; i++){
      bytes.push_back(charArray[i]);
    }

    f << "Descifrando " << bytes.size() << " bytes:" << endl;

    DynArray descifrado = decipher_bytes(&(bytes[0]), bytes.size());

    string recibido;

    for (int i=0;i <descifrado.length; i++) {
      recibido.push_back((char)descifrado.array[i]);
    }


    f << "Contenidos: " << recibido << endl;

    f.close();

    msg->str(recibido);
  }
};

#endif //USUARIOS_COM_H
