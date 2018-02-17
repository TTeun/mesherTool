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
  void showMesh();

private:
  void normalizePts(std::vector<Vertex> &pts);
  void buildInnerSquare(Config &config);
  void buildCircleQuadrant(Config &config, size_t quadrant);

  void buildQuadrantFaces(
      Config &config, size_t indexOffset, long indexIncrement, size_t connectingIndex, size_t quadrant);
  void buildSquareQuadrantVertices(Config &config, size_t quadrant);

  void buildMesh(Config &config);

  std::vector<Vertex *> _vertices;
  std::vector<Face *>   _faces;
};