#ifndef __GEO2D_H
#define __GEO2D_H

#include <SFML/Graphics.hpp>
#include <array>

struct Vertex2D : public sf::Vertex {
  Vertex2D(sf::Vector2f const &pos, size_t idx) : sf::Vertex(pos), _index(idx) {}

  size_t _index;
};

struct Edge2D;

class Face2D {
 public:
  Face2D(Vertex2D *vertex0, Vertex2D *vertex1, Vertex2D *vertex2, Vertex2D *vertex3)
      : _vertices({vertex0, vertex1, vertex2, vertex3}), _edges({nullptr, nullptr, nullptr, nullptr}) {}

  void addEdge(Edge2D *edge);

  sf::Vector2f getCenter();

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

#endif  //__GEO2D_H