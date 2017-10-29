#include "readfile.h"

namespace IO {
  void readNodes(vector<Type::Point> &pts){
    readFile("data/dat.node", pts);
  }

  void readPoly(vector<Type::Point> &quads, vector<Type::Point> &pts){
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
