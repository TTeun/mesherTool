#include "Mesh2D.h"

Mesh2D::Mesh2D() {}

void Mesh2D::showMesh()
{
  sf::RenderWindow        window(sf::VideoMode(500, 500), "Mesh!");
  std::vector<sf::Vertex> quads;
  for (auto faceIt = _faces.begin(); faceIt != _faces.end(); ++faceIt)
  {
    for (auto vertIt = (*faceIt)->begin(); vertIt != (*faceIt)->end(); ++vertIt)
    {
      quads.push_back(**vertIt);
    }
    quads.push_back(**(*faceIt)->begin());
  }

  std::vector<sf::Vertex> points;
  for (auto vertIt = _vertices.begin(); vertIt != _vertices.end(); ++vertIt)
  {
    points.push_back(**vertIt);
  }

  normalizePoints(points);
  normalizePoints(quads);
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }
    window.clear(sf::Color::Black);
    window.draw(points.data(), points.size(), sf::Points);
    for (size_t i = 0; i < quads.size(); i += 5)
    {
      window.draw(quads.data() + i, 5, sf::LinesStrip);
    }
    window.display();
  }
}

void Mesh2D::normalizePoints(std::vector<Vertex> &points)
{
  double xMin = std::numeric_limits<double>::max();
  double xMax = std::numeric_limits<double>::min();
  double yMin = std::numeric_limits<double>::max();
  double yMax = std::numeric_limits<double>::min();

  for (auto ptIt = points.begin(); ptIt != points.end(); ++ptIt)
  {
    xMin = std::min(xMin, static_cast<double>((*ptIt).position.x));
    xMax = std::max(xMax, static_cast<double>((*ptIt).position.x));
    yMin = std::min(yMin, static_cast<double>((*ptIt).position.y));
    yMax = std::max(yMax, static_cast<double>((*ptIt).position.y));
  }

  xMin *= 1.1;
  xMax *= 1.1;
  yMin *= 1.1;
  yMax *= 1.1;
  for (auto &pt : points)
  {
    pt.position.x -= xMin;
    pt.position.x *= 500.f / (xMax - xMin);
    pt.position.y -= yMin;
    pt.position.y *= 500.f / (yMax - yMin);
  }
}
