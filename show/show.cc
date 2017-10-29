#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <climits>
#include <cmath>
#include <string>
#include "IO/readfile.h"
#include "types/point.h"

using namespace std;

void normalizePts(vector<Type::Point> &pts){
  float x_min = numeric_limits<float>::max();
  float x_max = numeric_limits<float>::min();
  float y_min = numeric_limits<float>::max();
  float y_max = numeric_limits<float>::min();

  for (auto &pt : pts){
    x_min = min(x_min, pt.position.x);
    x_max = max(x_max, pt.position.x);
    y_min = min(y_min, pt.position.y);
    y_max = max(y_max, pt.position.y);
  }
  x_min--;
  x_max++;
  y_min--;
  y_max++;

  for (auto &pt : pts){
    pt.position.x -= x_min;
    pt.position.x *= 800.0 / (x_max - x_min);

    pt.position.y -= y_min;
    pt.position.y *= 600 / (y_max - y_min);
  }
}

int main()
{
  sf::RenderWindow window(sf::VideoMode(800, 600), "Mesh!");
  vector<Type::Point> pts;
  try {
    IO::readNodes(pts);
    cout << "Nodes read succesfully\n";
  }
  catch (string &str){
    cout << str << '\n';
  }

  normalizePts(pts);
  vector<Type::Point> quads;
  try {
    IO::readPoly(quads, pts);
    cout << "Polygons read succesfully\n";
  }
  catch (string &str){
    cout << str << '\n';
  }

  while (window.isOpen())
  {
      sf::Event event;
      while (window.pollEvent(event))
      {
          if (event.type == sf::Event::Closed)
              window.close();
      }

      window.clear(sf::Color::Black);
      for (size_t i = 0; i < quads.size(); i += 5)
        window.draw(quads.data() + i, 5, sf::LinesStrip);
      window.display();
  }
  return 0;
}
