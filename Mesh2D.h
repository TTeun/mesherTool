#ifndef __MESH2D_H
#define __MESH2D_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include "BaseConfig.h"

struct Vertex2D : public sf::Vertex {
  Vertex2D(sf::Vector2f const &pos, size_t idx) : sf::Vertex(pos), _index(idx) {}

  size_t _index;
};

typedef std::array<Vertex2D *, 4> Face2D;

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

  std::vector<Vertex2D *> &getVertices() { return _vertices; }

  std::vector<Vertex2D *> const &             getVertices() const { return _vertices; }
  std::vector<std::unique_ptr<Face2D>> const &getFaces() const { return _faces; }
  edgeMap const &                             getEdges() const { return _edges; }

  void showMesh();

 private:
  void addEdgesFromFace(Face2D *addedFace);
  void normalizePoints(std::vector<sf::Vertex> &points,
                       std::vector<sf::Vertex> &quads,
                       const double             scaler = 1.1);

  edgeMap                              _edges;
  std::vector<Vertex2D *>              _vertices;
  std::vector<std::unique_ptr<Face2D>> _faces;
};

#endif  // __MESH2D_H