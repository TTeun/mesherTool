#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <climits>
#include <cmath>
#include "../IO/io.h"
#include "../mesher/mesher.h"
#include "../exc/exc.h"
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
    pt.position.x *= 1000.f / (x_max - x_min);

    pt.position.y -= y_min;
    pt.position.y *= 1000.f / (y_max - y_min);
  }
}

int main()
{
  try {
    Mesher::Config config;
    Mesher::buildMesh(config);
  } catch (Exc &e){
    cout << e << '\n';
    return 1;
  }

  sf::RenderWindow window(sf::VideoMode(1000, 1000), "Mesh!");
  vector<Type::Point> pts;
  try {
    IO::readNodes("data/dat.node", pts);
    cout << pts.size() << " Nodes read succesfully\n";
  }
  catch (Exc &e){
    cout << e << '\n';
    window.close();
    return 1;
  }

  normalizePts(pts);
  vector<Type::Point> quads;
  try {
    IO::readPoly("data/dat.poly", quads, pts);
    cout << quads.size() / 5 << " Polygons read succesfully\n";
  }
  catch (Exc &e){
    cout << e << '\n';
    window.close();
    return 1;
  }

  vector<sf::Vertex> orientation;
  for (size_t i = 0; i < quads.size(); i += 5)
  {
    sf::Vertex vtx;
    vtx = (quads[i].position + 6.0f * quads[i + 1].position + quads[i + 2].position) / 8.0f;
    orientation.push_back(vtx);

    vtx = (quads[i + 1].position + 6.0f * quads[i + 2].position + quads[i + 3].position) / 8.0f;
    vtx.color = {255, 0, 0};
    orientation.push_back(vtx);

    vtx = (quads[i + 2].position + 6.0f * quads[i + 3].position + quads[i + 4].position) / 8.0f;
    vtx.color = {0, 0, 255};
    orientation.push_back(vtx);
  }

  bool showOrientation = false;
  while (window.isOpen())
  {
      sf::Event event;
      while (window.pollEvent(event))
      {
          if (event.type == sf::Event::Closed)
              window.close();
          if (event.type == sf::Event::EventType::KeyPressed)
          {
            if (event.key.code == sf::Keyboard::O)
            {
              showOrientation = not showOrientation;
            }
          }

      }

      window.clear(sf::Color::Black);
      for (size_t i = 0; i < quads.size(); i += 5)
        window.draw(quads.data() + i, 5, sf::LinesStrip);

      if (showOrientation)
        for (size_t i = 0; i < orientation.size(); i += 3)
          window.draw(orientation.data() + i, 3, sf::LinesStrip);

      window.draw(pts.data(), pts.size(), sf::Points);
      window.display();
  }
  return 0;
}
