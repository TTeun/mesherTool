#ifndef __MESH2D_H
#define __MESH2D_H

#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include "BaseConfig.h"
#include "Geo2D.h"

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

  void addFace(size_t idx0, size_t idx1, size_t idx2, size_t idx3, Face2D::FaceType type);
  void addVertex(const double x, const double y) {
    _vertices.push_back(std::unique_ptr<Vertex2D>(new Vertex2D(sf::Vector2f(x, y), _vertices.size())));
  }

  std::vector<std::unique_ptr<Vertex2D>> const &getVertices() const { return _vertices; }
  std::vector<std::unique_ptr<Face2D>> const &  getFaces() const { return _faces; }
  edgeMap const &                               getEdges() const { return _edges; }

  void showMesh();

 private:
  void addEdgesFromFace(Face2D *addedFace);

  edgeMap                                _edges;
  std::vector<std::unique_ptr<Vertex2D>> _vertices;
  std::vector<std::unique_ptr<Face2D>>   _faces;
};

#endif  // __MESH2D_H