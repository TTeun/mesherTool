#include "Geo2D.h"
#include <cassert>

void Face2D::addEdge(Edge2D *edge) {
  assert(edge != nullptr);
  auto edgeIt = std::find(_edges.begin(), _edges.end(), nullptr);
  assert(edgeIt != _edges.end());
  *edgeIt = edge;
}

sf::Vector2f Face2D::getCenter() const {
  sf::Vector2f result(0., 0.);
  for (auto vertIt = _vertices.begin(); vertIt != _vertices.end(); ++vertIt) {
    result += (*vertIt)->position;
  }
  result /= 4.f;
  return result;
}
