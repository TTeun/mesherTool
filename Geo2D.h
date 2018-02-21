#ifndef __GEO2D_H
#define __GEO2D_H

#include <array>
#include <tuple>

typedef std::pair<double, double> doublePair;

doublePair operator*(const double m, doublePair const &c);

doublePair operator+(const doublePair &lhs, const doublePair &rhs);

struct Vertex2D {
  Vertex2D(const double x, const double y, size_t idx) : _x(x), _y(y), _index(idx) {}

  operator doublePair() { return std::make_pair(_x, _y); }

  double _x;
  double _y;
  size_t _index;
};

struct Edge2D;

class Face2D {
 public:
  enum FaceType { Nozzle, Pipe, Outer };

  Face2D(Vertex2D *vertex0, Vertex2D *vertex1, Vertex2D *vertex2, Vertex2D *vertex3, FaceType type)
      : _vertices({vertex0, vertex1, vertex2, vertex3}),
        _edges({nullptr, nullptr, nullptr, nullptr}),
        _type(type) {}

  void addEdge(Edge2D *edge);

  doublePair getCenter() const;

  FaceType getType() const { return _type; }

  std::array<Vertex2D *, 4> const &getVertices() const { return _vertices; }
  std::array<Edge2D *, 4> const &  getEdges() const { return _edges; }

 private:
  std::array<Vertex2D *, 4> _vertices;
  std::array<Edge2D *, 4>   _edges;
  FaceType                  _type;
};

struct Edge2D {
  Edge2D(Face2D *newFace) {
    _faces[0] = newFace;
    _faces[1] = nullptr;
  }
  std::array<Face2D *, 2> _faces;
};

#endif  //__GEO2D_H