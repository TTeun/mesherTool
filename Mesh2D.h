#ifndef __MESH2D_H
#define __MESH2D_H

#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include "BaseConfig.h"

struct Vertex2D : public sf::Vertex {
  Vertex2D(sf::Vector2f const &pos, size_t idx) : sf::Vertex(pos), _index(idx) {}

  size_t _index;
};

struct Edge2D;

class Face2D {
 public:
  Face2D(Vertex2D *vertex0, Vertex2D *vertex1, Vertex2D *vertex2, Vertex2D *vertex3)
      : _vertices({vertex0, vertex1, vertex2, vertex3}), _edges({nullptr, nullptr, nullptr, nullptr}) {}
  void addEdge(Edge2D *edge) {
    assert(edge != nullptr);
    auto edgeIt = std::find(_edges.begin(), _edges.end(), nullptr);
    assert(edgeIt != _edges.end());
    *edgeIt = edge;
  }

  std::array<Vertex2D *, 4> const &getVertices() const { return _vertices; }
  std::array<Edge2D *, 4> const &  getEdges() const { return _edges; }

 private:
  std::array<Vertex2D *, 4> _vertices;
  std::array<Edge2D *, 4>   _edges;
};

struct Edge2D {
  Edge2D(Face2D *newFace) {
    _faces[0] = newFace;
    _faces[1] = nullptr;
  }
  std::array<Face2D *, 2> _faces;
};

struct pair_hash {
  template <class T1>
  std::size_t operator()(const std::pair<T1, T1> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    return h1 ^ (0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};

class Mesh2D {
  typedef std::unordered_map<std::pair<size_t, size_t>, std::unique_ptr<Edge2D>, pair_hash> edgeMap;

 public:
  Mesh2D();
  Mesh2D(Mesh2D &&other) : _vertices(std::move(other._vertices)), _faces(std::move(other._faces)) {}

  void addFace(size_t idx0, size_t idx1, size_t idx2, size_t idx3);
  void addVertex(const double x, const double y) {
    _vertices.push_back(std::unique_ptr<Vertex2D>(new Vertex2D(sf::Vector2f(x, y), _vertices.size())));
  }

  std::vector<std::unique_ptr<Vertex2D>> const &getVertices() const { return _vertices; }
  std::vector<std::unique_ptr<Face2D>> const &  getFaces() const { return _faces; }
  edgeMap const &                               getEdges() const { return _edges; }

  void showMesh();

 private:
  void addEdgesFromFace(Face2D *addedFace);

  void normalizePoints(std::vector<sf::Vertex> &points,
                       std::vector<sf::Vertex> &quads,
                       const double             scaler = 1.1);

  edgeMap                                _edges;
  std::vector<std::unique_ptr<Vertex2D>> _vertices;
  std::vector<std::unique_ptr<Face2D>>   _faces;
};

#endif  // __MESH2D_H