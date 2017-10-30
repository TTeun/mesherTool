#ifndef __MESH_BUILDER__H
#define __MESH_BUILDER__H

#include <fstream>
#include "config.h"

using namespace std;

namespace MeshBuilder {
  typedef pair<float, float>(*p_func)(float r, float half_square_size, float h);

  void buildMesh(Config &config);
  size_t buildQuadrant(
                    Config &config,
                    size_t index_offset,
                    long index_increment,
                    size_t connecting_index,
                    p_func f
                  );
  void buildInnerSquare(Config &config);
};

#endif //__MESH_BUILDER__H
