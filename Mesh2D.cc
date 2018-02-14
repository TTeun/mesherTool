#include "Mesh2D.h"
#include <climits>
#include <cmath>


Mesh2D::Mesh2D()
{
  Config cf;
  buildMesh(cf);
}

void Mesh2D::showMesh()
{
  sf::RenderWindow window(sf::VideoMode(1000, 1000), "Mesh!");
  std::vector<sf::Vertex> quads;
  for (auto faceIt = _faces.begin(); faceIt != _faces.end(); ++faceIt) {
    for (auto vertIt = (*faceIt)->begin(); vertIt != (*faceIt)->end(); ++vertIt) {
      quads.push_back(**vertIt);
    }
    quads.push_back(**(*faceIt)->begin());
  }
  std::vector<sf::Vertex> points;
  for (auto vertIt = _vertices.begin(); vertIt != _vertices.end(); ++vertIt) {
    points.push_back(**vertIt);
  }
  normalizePts(points);
  normalizePts(quads);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    window.clear(sf::Color::Black);
    window.draw(points.data(), points.size(), sf::Points);
    for (size_t i = 0; i < quads.size(); i += 5) {
      window.draw(quads.data() + i, 5, sf::LinesStrip);
    }
    window.display();
  }
}

void Mesh2D::normalizePts(std::vector<Vertex> &pts)
{
  float x_min = std::numeric_limits<float>::max();
  float x_max = std::numeric_limits<float>::min();
  float y_min = std::numeric_limits<float>::max();
  float y_max = std::numeric_limits<float>::min();
  for (auto &pt : pts) {
    x_min = std::min(x_min, pt.position.x);
    x_max = std::max(x_max, pt.position.x);
    y_min = std::min(y_min, pt.position.y);
    y_max = std::max(y_max, pt.position.y);
  }
  x_min--;
  x_max++;
  y_min--;
  y_max++;
  for (auto &pt : pts) {
    pt.position.x -= x_min;
    pt.position.x *= 1000.f / (x_max - x_min);
    pt.position.y -= y_min;
    pt.position.y *= 1000.f / (y_max - y_min);
  }
}

template <typename T>
T sq(T a)
{ return a * a; }

void Mesh2D::buildInnerSquare(Config &cf)
{
  for (long col = -static_cast<long>(cf.half_inner_block_count);
       col != static_cast<long>(cf.half_inner_block_count) + 1;
       ++col) {
    double abs_x = abs(col) / static_cast<double>(cf.half_inner_block_count);
    for (long row = -static_cast<long>(cf.half_inner_block_count);
         row != static_cast<long>(cf.half_inner_block_count) + 1;
         ++row) {
      double abs_y = abs(row) / static_cast<double>(cf.half_inner_block_count);
      _vertices.push_back(new Vertex(sf::Vector2f((1.0 - cf.alpha * sq(abs_x)) * row * cf.inner_block_width,
                                     (1.0 - cf.alpha * sq(abs_y)) * col * cf.inner_block_width)));
    }
  }
  for (size_t col = 0; col != cf.inner_block_count; ++col) {
    for (size_t row = 0; row != cf.inner_block_count; ++row) {
      size_t base_index = row * (cf.inner_block_count + 1) + col;
      _faces.push_back(new Face {
        _vertices[base_index],
        _vertices[base_index + 1],
        _vertices[base_index + cf.inner_block_count + 2],
        _vertices[base_index + cf.inner_block_count + 1] });
    }
  }
}

void Mesh2D::buildCircleQuadrant(Config &cf, size_t qdrnt)
{
  for (long col = -static_cast<long>(cf.half_inner_block_count);
       col != static_cast<long>(cf.half_inner_block_count);
       ++col) {
    /// Math....
    double s     = col / static_cast<double>(cf.half_inner_block_count);
    double abs_x = abs(s);
    double h_p   = s * cf.half_square_size;
    double d_p   = sqrt(h_p * h_p + cf.half_square_size * cf.half_square_size);
    double phi   = M_PI * s / 4.0;
    double h_a   = tan(phi) * cf.half_square_size;
    double d_a   = sqrt(h_a * h_a + cf.half_square_size * cf.half_square_size);
    double x_p, y_p, x_a, y_a;
    for (long rad = 1; rad != static_cast<long>(cf.outer_steps); ++rad) {
      float blend    = static_cast<double>(rad - 1.0) / static_cast<double>(cf.outer_steps - 2);
      double abs_rad = cf.alpha_connection * (1.0 - blend);
      double r_p = (1 + (cf.circle_radius / d_p - 1) * static_cast<double>(rad)
                    / static_cast<double>(cf.outer_steps - 1));
      double r_a = (1 + (cf.circle_radius / d_a - 1) * static_cast<double>(rad)
                    / static_cast<double>(cf.outer_steps - 1));
      x_p = r_p * (1 - cf.alpha * sq(abs_x) * abs_rad) * cf.half_square_size;
      y_p = r_p * (1 - abs_rad * cf.alpha) * h_p;
      x_a = r_a * (1 - cf.alpha * sq(abs_x) * abs_rad) * cf.half_square_size;
      y_a = r_a * (1 - abs_rad * cf.alpha) * h_a;
      double x = (1.0 - blend) * x_p + blend * x_a;
      double y = (1.0 - blend) * y_p + blend * y_a;
      for (size_t q = 0; q != qdrnt; ++q) {
        std::tie(x, y) = std::make_pair(-y, x);
      }
      _vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }
  }
}

void Mesh2D::buildSquareQuadrantVertices(Config &cf, size_t qdrnt)
{
  for (long col = -static_cast<long>(cf.half_inner_block_count);
       col != static_cast<long>(cf.half_inner_block_count);
       ++col) {
    for (long row = 1; row != static_cast<long>(cf.outer_steps); ++row) {
      double y = col;
      double x = 0.5 * cf.inner_square_side + row * cf.inner_block_width;
      for (size_t q = 0; q != qdrnt; ++q) {
        std::tie(x, y) = std::make_pair(-y, x);
      }
      _vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }
  }
}

void Mesh2D::buildQuadrantFaces( // The horror
  Config &cf,
  size_t index_offset,
  long index_increment,
  size_t connecting_index,
  size_t qdrnt)
{
  for (long col = 0; col != static_cast<long>(cf.inner_block_count) - 1; ++col) {
    for (long rad = 0; rad + 2 != static_cast<long>(cf.outer_steps); ++rad) {
      size_t base_index_inner = col * (cf.outer_steps - 1) + rad + index_offset;
      _faces.push_back(new Face {
        _vertices[base_index_inner],
        _vertices[base_index_inner + 1],
        _vertices[base_index_inner + cf.outer_steps],
        _vertices[base_index_inner + cf.outer_steps - 1] });
    }
    if (qdrnt == 0 || qdrnt == 3) {
      size_t base_index       = col * index_increment + connecting_index;
      size_t base_index_inner = col * (cf.outer_steps - 1) + index_offset;
      _faces.push_back(new Face {
        _vertices[base_index],
        _vertices[base_index_inner],
        _vertices[base_index_inner + cf.outer_steps - 1],
        _vertices[base_index + index_increment] });
    } else if (qdrnt == 1 || qdrnt == 2) {
      size_t base_index       = connecting_index - col * index_increment;
      size_t base_index_inner = col * (cf.outer_steps - 1) + index_offset;
      _faces.push_back(new Face {
        _vertices[base_index_inner],
        _vertices[base_index + index_increment] ,
        _vertices[base_index],
        _vertices[base_index_inner + cf.outer_steps - 1] });
    }
  }
  if (qdrnt < 3) {
    if (qdrnt == 0) {
      for (long rad = 0; rad + 2 != static_cast<long>(cf.outer_steps); ++rad) {
        size_t base_index_inner = rad + index_offset;
        _faces.push_back(new Face {
          _vertices[base_index_inner],
          _vertices[base_index_inner + 1],
          _vertices[base_index_inner + (cf.inner_block_count - 1) * (cf.outer_steps - 1) + 3 * (cf.inner_block_count ) * (cf.outer_steps - 1) + 1 ],
          _vertices[base_index_inner + (cf.inner_block_count - 1) * (cf.outer_steps - 1) + 3 * (cf.inner_block_count ) * (cf.outer_steps - 1)] });
      }
    }
    for (long rad = 0; rad + 2 != static_cast<long>(cf.outer_steps); ++rad) {
      long col = static_cast<long>(cf.inner_block_count) - 1;
      size_t base_index_inner = col * (cf.outer_steps - 1) + rad + index_offset;
      _faces.push_back(new Face {
        _vertices[base_index_inner],
        _vertices[base_index_inner + 1],
        _vertices[base_index_inner + cf.outer_steps],
        _vertices[base_index_inner + cf.outer_steps - 1] });
    }
  }
}
void Mesh2D::buildMesh(Config & cf)
{
  buildInnerSquare(cf);
  auto index_increments = std::vector<size_t> {
    cf.inner_block_count + 1,
    1,
    cf.inner_block_count + 1,
    1
  };
  auto connecting_indices = std::vector<size_t> {
    cf.inner_block_count,
    cf.inner_block_count * (cf.inner_block_count + 1) + cf.inner_block_count - 1,
    (cf.inner_block_count - 1) * (cf.inner_block_count + 1),
    0
  };
  size_t index_offset = (cf.inner_block_count + 1) * (cf.inner_block_count + 1);
  for (size_t qdrnt = 0; qdrnt != 4; ++qdrnt) {
    buildCircleQuadrant(
      cf,
      qdrnt
    );
    index_offset += (cf.inner_block_count + 1) * (cf.outer_steps - 1);
  }
  index_offset = (cf.inner_block_count + 1) * (cf.inner_block_count + 1);
  for (size_t qdrnt = 0; qdrnt != 4; ++qdrnt) {
    buildQuadrantFaces(
      cf,
      index_offset,
      index_increments[qdrnt],
      connecting_indices[qdrnt],
      qdrnt
    );
    index_offset += cf.inner_block_count * (cf.outer_steps - 1);
  }
  // auto index_increments = std::vector<size_t> {
  //   cf.inner_block_count + 1,
  //   1,
  //   cf.inner_block_count + 1,
  //   1
  // };
  // auto connecting_indices = std::vector<size_t> {
  //   cf.inner_block_count,
  //   cf.inner_block_count * (cf.inner_block_count + 1),
  //   0,
  //   0
  // };
  // size_t index_offset = (cf.inner_block_count + 1) * (cf.inner_block_count + 1);
  // for (size_t qdrnt = 0; qdrnt != 4; ++qdrnt) {
  //   buildCircleQuadrant(
  //     cf,
  //     index_offset,
  //     index_increments[qdrnt],
  //     connecting_indices[qdrnt],
  //     position_functions[qdrnt]
  //   );
  //   index_offset += (cf.inner_block_count + 1) * (cf.outer_steps - 1);
  // }
}