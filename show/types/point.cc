#include "point.h"

using namespace std;

namespace Type {
  Point::Point() : sf::Vertex(){}
  Point::Point(float _x, float _y)
  : sf::Vertex(sf::Vector2f(_x, _y), sf::Color(108, 196, 23))
  {}
  Point::Point(stringstream &ss)
  {
    ss >> position.x >> position.y;
    color = {135,207,62};
  }
} // Type
