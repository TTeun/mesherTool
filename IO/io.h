#ifndef __IO__H
#define __IO__H

#include <fstream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "../show/types/point.h"

using namespace std;

namespace IO {
  ofstream open_ofstream(char const * path, ios_base::openmode mode = ios_base::out);
  ifstream open_ifstream(char const * path, ios_base::openmode mode = ios_base::in);

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

  template<typename T>
  void readNodes(vector<T> &pts){
    readFile("data/dat.node", pts);
  }

  template<typename T>
  void readPoly(vector<T> &quads, vector<T> &pts){
    ifstream poly("data/dat.poly", fstream::in);
    string line;
    if (poly.is_open()){
      if (not getline(poly, line))
        throw string("Cant read header");

      stringstream ss(line);
      size_t size;
      size_t a, b, c, d;
      ss >> size;
      for (size_t i = 0; i != size; ++i){
        if (not getline(poly, line))
          throw string("Cant read data");

        stringstream  lineStream(line);
        lineStream >> a >> b >> c >> d;
        quads.push_back(pts[a]);
        quads.push_back(pts[b]);
        quads.push_back(pts[c]);
        quads.push_back(pts[d]);
        quads.push_back(pts[a]);
      }
      poly.close();
    }
    else {
      throw string("Cant open");
    }
  }

} // IO

#endif // __IO
