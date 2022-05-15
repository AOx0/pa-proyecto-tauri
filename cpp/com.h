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
private:
  string file;

public:
  explicit Communicator(string file) : file(move(file)) {}

  void send(stringstream * msg) {
    string message = msg->str();

    std::ofstream ofs;
    ofs.open(file, std::ofstream::out | std::ofstream::trunc);

    vector<uint8_t> bytes;
    for (auto c: message) {
      bytes.push_back((uint8_t)c);
    }

    DynArray cifrado = cypher_bytes(&(bytes[0]), bytes.size());

    vector<char> to_write = vector<char>();

    for (int i=0; i<cifrado.length; i++) {
      to_write.push_back((char)cifrado.array[i]);
    }

    ofs.write(&(to_write[0]), to_write.size());

    ofs.close();
  }

  void receive(stringstream * msg) {
    std::ifstream ifs;
    ifs.open(file, ifstream::binary);

    vector<char> written = vector<char>();
    vector<uint8_t> bytes;

    vector<char> data = vector<char>(istreambuf_iterator<char>(ifs),(istreambuf_iterator<char>()));
    char* charArray = &data[0];
    size_t arraySize = data.size();

    for (int i=0; i < arraySize; i++){
      bytes.push_back(charArray[i]);
    }

    DynArray descifrado = decipher_bytes(&(bytes[0]), bytes.size());

    string recibido;

    for (int i=0;i <descifrado.length; i++) {
      recibido.push_back((char)descifrado.array[i]);
    }

    msg->str(recibido);
  }
};

#endif //USUARIOS_COM_H
