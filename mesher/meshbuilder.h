#ifndef __MESH_BUILDER__H
#define __MESH_BUILDER__H

#include <fstream>
#include "config.h"

using namespace std;

namespace MeshBuilder {
  typedef pair<double, double>(*p_func)(double r, double half_square_size, double h);

  void buildMesh(Config &config);
  void buildInnerSquare(Config &config);
  void buildQuadrant(
                    Config &config,
                    size_t index_offset,
                    long index_increment,
                    size_t connecting_index,
                    p_func f,
                    bool invert_ortient
                  );
  inline void printPoly(
                ofstream &poly,
                size_t a,
                size_t b,
                size_t c,
                size_t d,
                bool invert_orient
              )
  {
    if (not invert_orient)
      poly << a << " " << b << " " << c << " " << d << '\n';
    else
      poly << d << " " << c << " " << b << " " << a << '\n';
  }

}

#endif //__MESH_BUILDER__H
