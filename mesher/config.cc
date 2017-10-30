#include "config.h"
#include "../IO/io.h"
#include <cmath>

using namespace std;

namespace MeshBuilder {
  Config::Config(){
    inner_square_side     = 3.0;
    inner_block_min_width = 0.4;
    circle_radius         = 4.0;
    inner_block_count     = ceil(inner_square_side / inner_block_min_width);
    outer_steps           = 8;
    if (inner_block_count % 2)
      ++inner_block_count;

    node = IO::open_ofstream("data/dat.node", std::ofstream::out | std::ofstream::trunc);
    poly = IO::open_ofstream("data/dat.poly", std::ofstream::out | std::ofstream::trunc);
  }

  void Config::printHeaders(){
    node << (inner_block_count + 1) * (inner_block_count + 1) + 4 * (outer_steps - 1) * (inner_block_count + 1) << '\n';
    poly << inner_block_count * inner_block_count + 4 * (outer_steps - 1) * inner_block_count << '\n';
  }


} // MeshBuilder
