#ifndef __POINT__H
#define __POINT__H

#include <SFML/Graphics.hpp>
#include <sstream>

namespace Type {
  class Point : public sf::Vertex {
  public:
    Point();
    Point(float _x, float _y);
    Point(std::stringstream &ss);
  };


} // Type

#endif // __POINT__H
