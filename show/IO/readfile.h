#ifndef __READFILE__H
#define __READFILE__H

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "../types/point.h"

using namespace std;

namespace IO {
  template<typename T>
  void readFile(char const *path, vector<T> &vec){
    ifstream file(path, fstream::in);
    string line;
    if (file.is_open()){
      if (not getline(file, line))
        throw string("Cant read header");

      stringstream ss(line);
      size_t size;
      ss >> size;
      for (size_t i = 0; i != size; ++i){
        if (not getline(file, line))
          throw string("Cant read data");

        stringstream  lineStream(line);
        vec.push_back(T(lineStream));
      }
      file.close();
    }
    else {
      throw string("Cant open");
    }
  }

  void readNodes(vector<Type::Point> &pts);
  void readPoly(vector<Type::Point> &quads, vector<Type::Point> &pts);
} // IO

#endif // __READFILE__H
