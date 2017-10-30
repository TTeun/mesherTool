#ifndef __CONFIG__H
#define __CONFIG__H

#include <fstream>
#include <cstddef>

namespace MeshBuilder {

  struct Config {
    Config();

    float inner_square_side;
    float inner_block_min_width;
    size_t inner_block_count;
    size_t outer_steps;
    float circle_radius;
    std::ofstream node;
    std::ofstream poly;

    void printHeaders();
  };
} // MeshBuilder


#endif //__CONFIG__H
