#include "io.h"
#include "../exc/exc.h"

namespace IO {
  ofstream open_ofstream(char const * path, ios_base::openmode mode){
    ofstream str(path, mode);
    if (not str.is_open()){
      string error("Could not open file ");
      error.append(path);
      throw Exc(std::move(error), Exc::EXC_TYPE::ERROR);
    }
    return str;
  }

  ifstream open_ifstream(char const * path, ios_base::openmode mode){
    ifstream str(path, mode);
    if (not str.is_open()){
      string error("Could not open file ");
      error.append(path);
      throw Exc(std::move(error), Exc::EXC_TYPE::ERROR);
    }
    return str;
  }

  void readNodes(char const *path, vector<Type::Point> &pts){
    ifstream node = open_ifstream(path, fstream::in);
    string line;
    if (not getline(node, line)){
      string error("Cant read header of ");
      error.append(path);
      throw Exc(move(error), Exc::EXC_TYPE::ERROR);
    }
    stringstream ss(line);
    size_t size;
    ss >> size;
    for (size_t i = 0; i != size; ++i){
      if (not getline(node, line))
      {
        string error("Cant read data ");
        error.append(line);
        error.append(" of ");
        error.append(path);
        throw Exc(move(error), Exc::EXC_TYPE::ERROR);
      }

      stringstream lineStream(line);
      pts.push_back(Type::Point(lineStream));
    }
    node.close();
  }

  void readPoly(char const *path, vector<Type::Point> &quads, vector<Type::Point> &pts){
    ifstream poly = open_ifstream(path, fstream::in);
    string line;
    if (not getline(poly, line)){
      string error("Cant read header of ");
      error.append(path);
      throw Exc(move(error), Exc::EXC_TYPE::ERROR);
    }

    stringstream ss(line);
    size_t size;
    size_t a, b, c, d;
    ss >> size;
    for (size_t i = 0; i != size; ++i){
      if (not getline(poly, line))
      {
        string error("Cant read data ");
        error.append(line);
        error.append(" of ");
        error.append(path);
        throw Exc(move(error), Exc::EXC_TYPE::ERROR);
      }

      stringstream lineStream(line);
      lineStream >> a >> b >> c >> d;
      quads.push_back(pts[a]);
      quads.push_back(pts[b]);
      quads.push_back(pts[c]);
      quads.push_back(pts[d]);
      quads.push_back(pts[a]);
    }
    poly.close();
  }
} // IO
