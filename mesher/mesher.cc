#include "../IO/io.h"
#include "../exc/exc.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <tuple>

using namespace std;

void buildInnerSquare(
                     ofstream &node,
                     ofstream &poly,
                     size_t inner_block_count,
                     float inner_square_side
                   )
{
  float inner_block_width  = inner_square_side / inner_block_count;
  float half_square_size = inner_square_side / 2;

  for (long col = 0; col != static_cast<long>(inner_block_count) + 1; ++col)
    for (long row = 0; row < static_cast<long>(inner_block_count) + 1; ++row)
      node << row * inner_block_width - half_square_size << " " << col * inner_block_width - half_square_size << '\n';

  for (size_t col = 0; col != inner_block_count; ++col)
    for (size_t row = 0; row != inner_block_count; ++row)
    {
      size_t base_index = row * (inner_block_count + 1) + col;
      poly << base_index << " " << base_index + 1 << " " << base_index + inner_block_count + 2 << " " << base_index + inner_block_count + 1 << '\n';
    }
}

void buildQuadrant(
                  ofstream &node,
                  ofstream &poly,
                  size_t inner_block_count,
                  float inner_square_side,
                  float circle_radius,
                  size_t outer_steps,
                  size_t index_offset,
                  size_t index_increment,
                  size_t connecting_index,
                  pair<float, float>(*f)(float r, float half_square_size, float h)
                )
{
  float half_square_size = inner_square_side / 2;
  size_t half_inner_block_count = (inner_block_count + 1) / 2;
  for (long row = 0; row != static_cast<long>(inner_block_count) + 1; ++row)
  {
    float h = static_cast<float>(row - static_cast<long>(half_inner_block_count)) * half_square_size / ( static_cast<float>(half_inner_block_count));
    float d = sqrt(h * h + half_square_size * half_square_size);
    float r_prime = circle_radius - d;
    for (long rad = 1; rad != static_cast<long>(outer_steps); ++rad)
    {
      float r = (d + r_prime * static_cast<float>(rad) / static_cast<float>(outer_steps - 1)) / d;
      auto [x, y] = f(r, half_square_size, h);
      node << x << " " << y << '\n';
    }
  }

  for (long row = 0; row != static_cast<long>(inner_block_count); ++row){
    for (long rad = 0; rad + 2 != static_cast<long>(outer_steps); ++rad)
    {
      size_t base_index_inner = row * (outer_steps - 1) + rad + index_offset;
      poly << base_index_inner << " " << base_index_inner + 1 << " " << base_index_inner + outer_steps << " " << base_index_inner + outer_steps - 1 << '\n';
    }
    size_t base_index = row * index_increment + connecting_index;
    size_t base_index_inner = row * (outer_steps - 1) + index_offset;
    poly << base_index << " " << base_index_inner << " " << base_index_inner + outer_steps - 1 << " " << base_index + inner_block_count + 1 << '\n';
  }
}

void writeInnerBlock(float inner_square_side, size_t inner_block_count, float circle_radius){
  ofstream node = IO::open_ofstream("data/dat.node", std::ofstream::out | std::ofstream::trunc);
  ofstream poly = IO::open_ofstream("data/dat.poly", std::ofstream::out | std::ofstream::trunc);

  size_t outer_steps = 12;

  node << (inner_block_count + 1) * (inner_block_count + 1) + (outer_steps - 1) * (inner_block_count + 1) << '\n';
  poly << inner_block_count * inner_block_count + (outer_steps - 1) * inner_block_count << '\n';

  buildInnerSquare(node, poly, inner_block_count, inner_square_side);
  buildQuadrant(
                node,
                poly,
                inner_block_count,
                inner_square_side,
                circle_radius,
                outer_steps,
                (inner_block_count + 1) * (inner_block_count + 1),
                (inner_block_count + 1),
                inner_block_count,
                [](float r, float half_square_size, float h){
                  return make_pair(r * half_square_size, r * h);
                }
              );
}

int main(){
  float inner_square_side     = 3.0;
  float inner_block_min_width = 0.4;
  float circle_radius         = 4.0;
  size_t inner_block_count    = ceil(inner_square_side / inner_block_min_width);
  try {
    writeInnerBlock(inner_square_side, inner_block_count, circle_radius);
  } catch (Exc &e){
    cout << e << '\n';
  }
}
