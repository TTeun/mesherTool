#ifndef __MESH2D_H
#define __MESH2D_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include "BaseConfig.h"

typedef sf::Vertex                Vertex2D;
typedef std::array<Vertex2D *, 4> Face2D;

struct Edge2D {
  Face2D *face1;
  Face2D *face2;
};

struct pair_hash {
  template <class T1>
  std::size_t operator()(const std::pair<T1, T1> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    return h1 ^ (0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};

class Mesh2D {
  typedef std::map<std::pair<Vertex2D *, Vertex2D *>, Edge2D *, pair_hash> edgeMap;

 public:
  Mesh2D();
  Mesh2D(Mesh2D &&other) : _vertices(std::move(other._vertices)), _faces(std::move(other._faces)) {}

  void addFace(size_t idx0, size_t idx1, size_t idx2, size_t idx3);

  std::vector<Vertex2D *> &getVertices() { return _vertices; }

  std::vector<Vertex2D *> const &getVertices() const { return _vertices; }
  std::vector<Face2D *> const &  getFaces() const { return _faces; }
  edgeMap const &                getEdges() const { return _edges; }

  void showMesh();

 private:
  void normalizePoints(std::vector<Vertex2D> &pts);

  edgeMap                 _edges;
  std::vector<Vertex2D *> _vertices;
  std::vector<Face2D *>   _faces;
};

#endif  // __MESH2D_H