#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "config.h"

typedef sf::Vertex Vertex;
typedef std::vector<Vertex*> Face;

class Mesh2D {
  public:
    Mesh2D();
    void showMesh();

  private:

    void normalizePts(std::vector<Vertex> &pts);
    void buildInnerSquare(Config &cf);
    void buildCircleQuadrant(Config &cf, size_t qdrnt);

    void buildQuadrantFaces(
      Config &cf,
      size_t index_offset,
      long index_increment,
      size_t connecting_index,
      size_t qdrnt
    );
    void buildSquareQuadrantVertices(Config &cf, size_t qdrnt);

    void buildMesh(Config &cf);

    std::vector<Vertex*> _vertices;
    std::vector<Face*>   _faces;
};