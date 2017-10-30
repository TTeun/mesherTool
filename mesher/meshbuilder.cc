#include "meshbuilder.h"
#include "../IO/io.h"
#include "../exc/exc.h"
#include <cmath>

using namespace std;

namespace MeshBuilder {
  void buildInnerSquare(Config &cf)
  {
    float inner_block_width  = cf.inner_square_side / cf.inner_block_count;
    float half_square_size = cf.inner_square_side / 2;

    for (long col = 0; col != static_cast<long>(cf.inner_block_count) + 1; ++col)
      for (long row = 0; row < static_cast<long>(cf.inner_block_count) + 1; ++row)
        cf.node << row * inner_block_width - half_square_size << " " << col * inner_block_width - half_square_size << '\n';

    for (size_t col = 0; col != cf.inner_block_count; ++col)
      for (size_t row = 0; row != cf.inner_block_count; ++row)
      {
        size_t base_index = row * (cf.inner_block_count + 1) + col;
        cf.poly << base_index << " " << base_index + 1 << " " << base_index + cf.inner_block_count + 2 << " " << base_index + cf.inner_block_count + 1 << '\n';
      }
  }

  size_t buildQuadrant(
                    Config &cf,
                    size_t index_offset,
                    long index_increment,
                    size_t connecting_index,
                    p_func f
                  )
  {
    size_t temp_index_offset      = index_offset;
    float half_square_size        = cf.inner_square_side / 2;
    size_t half_inner_block_count = (cf.inner_block_count + 1) / 2;
    for (long row = 0; row != static_cast<long>(cf.inner_block_count) + 1; ++row)
    {
      float h = static_cast<float>(row - static_cast<long>(half_inner_block_count)) * half_square_size / ( static_cast<float>(half_inner_block_count));
      float d = sqrt(h * h + half_square_size * half_square_size);
      float r_prime = cf.circle_radius - d;
      for (long rad = 1; rad != static_cast<long>(cf.outer_steps); ++rad)
      {
        float r = (d + r_prime * static_cast<float>(rad) / static_cast<float>(cf.outer_steps - 1)) / d;
        auto [x, y] = f(r, half_square_size, h);
        cf.node << x << " " << y << '\n';
        ++temp_index_offset;
      }
    }

    for (long row = 0; row != static_cast<long>(cf.inner_block_count); ++row){
      for (long rad = 0; rad + 2 != static_cast<long>(cf.outer_steps); ++rad)
      {
        size_t base_index_inner = row * (cf.outer_steps - 1) + rad + index_offset;
        cf.poly << base_index_inner << " " << base_index_inner + 1 << " " << base_index_inner + cf.outer_steps << " " << base_index_inner + cf.outer_steps - 1 << '\n';
      }
      size_t base_index = row * index_increment + connecting_index;
      size_t base_index_inner = row * (cf.outer_steps - 1) + index_offset;
      cf.poly << base_index << " " << base_index_inner << " " << base_index_inner + cf.outer_steps - 1 << " " << base_index + index_increment << '\n';
    }
    return temp_index_offset;
  }

  void buildMesh(Config &cf){
    cf.printHeaders();
    buildInnerSquare(cf);

    auto index_increments   = vector<size_t>{(cf.inner_block_count + 1), 1, (cf.inner_block_count + 1), 1};
    auto connecting_indices = vector<size_t>{cf.inner_block_count, (cf.inner_block_count) * (cf.inner_block_count + 1), 0, 0};
    auto position_functions = vector<p_func>{
      [](float r, float half_square_size, float h){return make_pair(r * half_square_size, r * h); },
      [](float r, float half_square_size, float h){return make_pair(r * h, r * half_square_size); },
      [](float r, float half_square_size, float h){return make_pair(-r * half_square_size, r * h); },
      [](float r, float half_square_size, float h){return make_pair(r * h, -r * half_square_size); }
    };
    size_t index_offset = (cf.inner_block_count + 1) * (cf.inner_block_count + 1);
    for (size_t qdrnt = 0; qdrnt != 4; ++qdrnt)
      index_offset = buildQuadrant(
                            cf,
                            index_offset,
                            index_increments[qdrnt],
                            connecting_indices[qdrnt],
                            position_functions[qdrnt]
                          );
  }
} // MeshBuilder
