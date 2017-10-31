#include "mesher.h"
#include "../IO/io.h"
#include "../exc/exc.h"
#include <cmath>

using namespace std;

template <typename T>
T sq(T a){
  return a * a;
}

namespace Mesher {

  void buildInnerSquare(Config &cf)
  {
    for (long col = -static_cast<long>(cf.half_inner_block_count);
              col != static_cast<long>(cf.half_inner_block_count) + 1;
            ++col)
    {
      double abs_x = abs(col) / static_cast<double>(cf.half_inner_block_count);
      for (long row = -static_cast<long>(cf.half_inner_block_count);
                row != static_cast<long>(cf.half_inner_block_count) + 1;
              ++row)
      {
        double abs_y = abs(row) / static_cast<double>(cf.half_inner_block_count);
        cf.node << (1.0 - cf.alpha * sq(abs_x)) * row * cf.inner_block_width << " "
                << (1.0 - cf.alpha * sq(abs_y)) * col * cf.inner_block_width << '\n';
      }
    }

    for (size_t col = 0; col != cf.inner_block_count; ++col)
      for (size_t row = 0; row != cf.inner_block_count; ++row)
      {
        size_t base_index = row * (cf.inner_block_count + 1) + col;
        printPoly(
                  cf.poly,
                  base_index,
                  base_index + 1,
                  base_index + cf.inner_block_count + 2,
                  base_index + cf.inner_block_count + 1,
                  false
                );
      }
  }

  void buildQuadrant(
                    Config &cf,
                    size_t index_offset,
                    long index_increment,
                    size_t connecting_index,
                    p_func pos_func,
                    bool invert_ortient
                  )
  {
    size_t temp_index_offset      = index_offset;
    for (long col = -static_cast<long>(cf.half_inner_block_count);
              col != static_cast<long>(cf.half_inner_block_count) + 1;
            ++col)
    {
      double s     = col / static_cast<double>(cf.half_inner_block_count);
      double abs_x = abs(s);

      double h_p   = s * cf.half_square_size;
      double d_p   = sqrt(h_p * h_p + cf.half_square_size * cf.half_square_size);

      double phi   = M_PI * s / 4.0;
      double h_a   = tan(phi) * cf.half_square_size;
      double d_a   = sqrt(h_a * h_a + cf.half_square_size * cf.half_square_size);

      for (long rad = 1; rad != static_cast<long>(cf.outer_steps); ++rad)
      {
        float blend    = static_cast<double>(rad - 1.0) / static_cast<double>(cf.outer_steps - 2);
        double abs_rad = cf.alpha_connection * (1.0 - blend);

        double r_p = (1 + (cf.circle_radius / d_p - 1) * static_cast<double>(rad)
                    / static_cast<double>(cf.outer_steps - 1));

        double r_a = (1 + (cf.circle_radius / d_a - 1) * static_cast<double>(rad)
                    / static_cast<double>(cf.outer_steps - 1));

        auto [x_p, y_p] = pos_func(r_p, (1 - cf.alpha * sq(abs_x) * abs_rad) * cf.half_square_size, (1 - abs_rad * cf.alpha) * h_p);
        auto [x_a, y_a] = pos_func(r_a, (1 - cf.alpha * sq(abs_x) * abs_rad) * cf.half_square_size, (1 - abs_rad * cf.alpha) * h_a);


        double x = (1.0 - blend) * x_p + blend * x_a;
        double y = (1.0 - blend) * y_p + blend * y_a;
        cf.node << x << " " << y << '\n';
        ++temp_index_offset;
      }
    }

    for (long col = 0; col != static_cast<long>(cf.inner_block_count); ++col)
    {
      for (long rad = 0; rad + 2 != static_cast<long>(cf.outer_steps); ++rad)
      {
        size_t base_index_inner = col * (cf.outer_steps - 1) + rad + index_offset;
        printPoly(
                  cf.poly,
                  base_index_inner,
                  base_index_inner + 1,
                  base_index_inner + cf.outer_steps,
                  base_index_inner + cf.outer_steps - 1,
                  invert_ortient
                );
      }
      size_t base_index       = col * index_increment + connecting_index;
      size_t base_index_inner = col * (cf.outer_steps - 1) + index_offset;
      printPoly(
                cf.poly,
                base_index,
                base_index_inner,
                base_index_inner + cf.outer_steps - 1,
                base_index + index_increment,
                invert_ortient
              );
    }
  }

  void buildMesh(Config &cf)
  {
    cf.printHeaders();
    buildInnerSquare(cf);
    auto index_increments = vector<size_t>
    {
      cf.inner_block_count + 1,
      1,
      cf.inner_block_count + 1,
      1
    };
    auto connecting_indices = vector<size_t>
    {
      cf.inner_block_count,
      cf.inner_block_count * (cf.inner_block_count + 1),
      0,
      0
    };
    auto invert_orient = vector<bool>{false, true, true, false};
    auto position_functions = vector<p_func>{
      [](double r, double half_square_size, double h){return make_pair(r * half_square_size, r * h); },
      [](double r, double half_square_size, double h){return make_pair(r * h, r * half_square_size); },
      [](double r, double half_square_size, double h){return make_pair(-r * half_square_size, r * h); },
      [](double r, double half_square_size, double h){return make_pair(r * h, -r * half_square_size); }
    };
    size_t index_offset = (cf.inner_block_count + 1) * (cf.inner_block_count + 1);
    for (size_t qdrnt = 0; qdrnt != 4; ++qdrnt)
    {
      buildQuadrant(
        cf,
        index_offset,
        index_increments[qdrnt],
        connecting_indices[qdrnt],
        position_functions[qdrnt],
        invert_orient[qdrnt]
      );
      index_offset += (cf.inner_block_count + 1) * (cf.outer_steps - 1);
    }
  }
} // Mesher
