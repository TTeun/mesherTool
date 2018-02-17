#ifndef __MESH2D_H
#define __MESH2D_H

#include "config.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

typedef sf::Vertex            Vertex;
typedef std::vector<Vertex *> Face;

class Mesh2D
{
public:
  Mesh2D();
  Mesh2D(Mesh2D &&other) : _vertices(std::move(other._vertices)), _faces(std::move(other._faces)) {}

  std::vector<Vertex *> &getVertices() { return _vertices; }
  std::vector<Face *> &  getFaces() { return _faces; }

  void showMesh();

private:
  void buildMesh(Config &config);
  void normalizePoints(std::vector<Vertex> &pts);
  void buildInnerSquare(Config &config);
  void buildCircleQuadrant(Config &config, size_t quadrant);
  void buildQuadrantFaces(
      Config &config, size_t indexOffset, long indexIncrement, size_t connectingIndex, size_t quadrant);
  void buildSquareQuadrantVertices(Config &config, size_t quadrant);

  std::vector<Vertex *> _vertices;
  std::vector<Face *>   _faces;
};

#endif // __MESH2D_H