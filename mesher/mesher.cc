#include "../IO/io.h"
#include "../exc/exc.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

void writeInnerBlock(float inner_square_side, size_t inner_block_count){
  ofstream node = IO::open_ofstream("data/dat.node", std::ofstream::out | std::ofstream::trunc);
  ofstream poly = IO::open_ofstream("data/dat.poly", std::ofstream::out | std::ofstream::trunc);

  node << (inner_block_count + 1) * (inner_block_count + 1) << '\n';
  poly << inner_block_count * inner_block_count << '\n';

  float inner_block_width  = inner_square_side / inner_block_count;
  for (size_t row = 0; row != inner_block_count + 1; ++row)
    for (size_t col = 0; col != inner_block_count + 1; ++col)
      node << row * inner_block_width << " " << col * inner_block_width << '\n';

  for (size_t row = 0; row != inner_block_count; ++row)
    for (size_t col = 0; col != inner_block_count; ++col)
    {
      size_t base_index = row * (inner_block_count + 1) + col;
      poly << base_index << " "
           << base_index + 1 << " "
           << base_index + 2 + inner_block_count << " "
           << base_index + 1 + inner_block_count << '\n';
    }
}

int main(){
  float inner_square_side     = 1.0;
  float inner_block_min_width = 0.2;
  size_t inner_block_count    = ceil(inner_square_side / inner_block_min_width);
  try {
    writeInnerBlock(inner_square_side, inner_block_count);
  } catch (Exc &e){
    cout << e << '\n';
  }
}
