#include "Geo2D.h"
#include <algorithm>
#include <cassert>

void Face2D::addEdge(Edge2D *edge) {
  assert(edge != nullptr);
  auto edgeIt = std::find(_edges.begin(), _edges.end(), nullptr);
  assert(edgeIt != _edges.end());
  *edgeIt = edge;
}

doublePair Face2D::getCenter() const {
  doublePair result = std::make_pair(0., 0.);
  for (auto vertIt = _vertices.begin(); vertIt != _vertices.end(); ++vertIt) {
    result = result + static_cast<doublePair>(**vertIt);
  }
  result = .25 * result;
  return result;
}

typedef std::pair<double, double> doublePair;

doublePair operator*(const double m, doublePair const &c) {
  doublePair r(c);
  r.first *= m;
  r.second *= m;
  return r;
}

doublePair operator+(const doublePair &lhs, const doublePair &rhs) {
  doublePair r(rhs);
  r.first += lhs.first;
  r.second += lhs.second;
  return r;
}
