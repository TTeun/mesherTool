#include "Mesh2D.h"
#include "CoordinateHelper.h"
#include <cassert>
#include <climits>
#include <cmath>

Mesh2D::Mesh2D()
{
  Config config;
  buildMesh(config);
  std::cout << _faces.size() << " faces and " << _vertices.size() << " vertices \n";
}

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

  normalizePts(points);
  normalizePts(quads);
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

void Mesh2D::normalizePts(std::vector<Vertex> &pts)
{
  double xMin = std::numeric_limits<double>::max();
  double xMax = std::numeric_limits<double>::min();
  double yMin = std::numeric_limits<double>::max();
  double yMax = std::numeric_limits<double>::min();

  for (auto ptIt = pts.begin(); ptIt != pts.end(); ++ptIt)
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
  for (auto &pt : pts)
  {
    pt.position.x -= xMin;
    pt.position.x *= 500.f / (xMax - xMin);
    pt.position.y -= yMin;
    pt.position.y *= 500.f / (yMax - yMin);
  }
}

void Mesh2D::buildInnerSquare(Config &config)
{
  for (long xIndex = -static_cast<long>(config.halfInnerBlockCount);
       xIndex != static_cast<long>(config.halfInnerBlockCount) + 1;
       ++xIndex)
  {
    double absX = std::abs(xIndex) / static_cast<double>(config.halfInnerBlockCount);
    for (long yIndex = -static_cast<long>(config.halfInnerBlockCount);
         yIndex != static_cast<long>(config.halfInnerBlockCount) + 1;
         ++yIndex)
    {
      double absY = std::abs(yIndex) / static_cast<double>(config.halfInnerBlockCount);
      _vertices.push_back(
          new Vertex(sf::Vector2f((1.0 - config.alpha * sq(absX)) * yIndex * config.innerBlockWidth,
                                  (1.0 - config.alpha * sq(absY)) * xIndex * config.innerBlockWidth)));
    }
  }

  for (size_t xIndex = 0; xIndex != config.innerBlockCount; ++xIndex)
  {
    for (size_t yIndex = 0; yIndex != config.innerBlockCount; ++yIndex)
    {
      size_t baseIndex = yIndex * (config.innerBlockCount + 1) + xIndex;
      _faces.push_back(new Face{_vertices[baseIndex],
                                _vertices[baseIndex + 1],
                                _vertices[baseIndex + config.innerBlockCount + 2],
                                _vertices[baseIndex + config.innerBlockCount + 1]});
    }
  }
}

void Mesh2D::buildCircleQuadrant(Config &config, size_t quadrant)
{
  CircleAroundSquareCoordinates circleAroundSquareCoordinates(config);
  CircleCoordinates             circ(config.nozzleRadius, config.pipeRadius, config.pipeSteps, config);
  SquareAroundCircleCoordinates sqr(config.pipeRadius, config.outerSquareSide, config.outerSteps, config);
  CoordinateHelper              coordinateHelper(config);

  double x;
  double y;
  double xS;
  double yS;
  for (long xIndex = -static_cast<long>(config.halfInnerBlockCount);
       xIndex != static_cast<long>(config.halfInnerBlockCount);
       ++xIndex)
  {
    /// Math....
    circleAroundSquareCoordinates.setValues(xIndex);
    circ.setValues(xIndex);
    sqr.setValues(xIndex);

    coordinateHelper.setValues(xIndex);
    for (long radialIndex = 1; radialIndex != static_cast<long>(config.nozzleSteps); ++radialIndex)
    {
      std::tie(x, y) = coordinateHelper.getCoords(radialIndex, CoordinateHelper::Region1, quadrant);
      _vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }

    for (long radialIndex = 1; radialIndex <= static_cast<long>(config.pipeSteps); ++radialIndex)
    {
      std::tie(x, y) = circ.getXY(xIndex, radialIndex);
      for (size_t q = 0; q != quadrant; ++q)
      {
        std::tie(x, y) = std::make_pair(-y, x);
      }
      _vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }

    double blend = 0.;
    for (long radialIndex = 1; radialIndex <= static_cast<long>(config.outerSteps); ++radialIndex)
    {
      std::tie(x, y)   = circ.getXY(xIndex, 0);
      std::tie(xS, yS) = sqr.getXY(xIndex, radialIndex + config.pipeSteps);
      std::tie(x, y)   = std::make_pair((1 - blend) * x + blend * xS, (1 - blend) * y + blend * yS);

      for (size_t q = 0; q != quadrant; ++q)
      {
        std::tie(x, y) = std::make_pair(-y, x);
      }
      blend += 1. / (config.outerSteps - 1);
      _vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }
  }
}

void Mesh2D::buildSquareQuadrantVertices(Config &config, size_t quadrant)
{
  for (long xIndex = -static_cast<long>(config.halfInnerBlockCount);
       xIndex != static_cast<long>(config.halfInnerBlockCount);
       ++xIndex)
  {
    for (long yIndex = 1; yIndex != static_cast<long>(config.totalSteps); ++yIndex)
    {
      double y = xIndex;
      double x = 0.5 * config.innerSquareSide + yIndex * config.innerBlockWidth;
      for (size_t q = 0; q != quadrant; ++q)
      {
        std::tie(x, y) = std::make_pair(-y, x);
      }
      _vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }
  }
}

void Mesh2D::buildQuadrantFaces( // The horror
    Config &config,
    size_t  indexOffset,
    long    indexIncrement,
    size_t  connectingIndex,
    size_t  quadrant)
{
  for (long xIndex = 0; xIndex != static_cast<long>(config.innerBlockCount) - 1; ++xIndex)
  {
    for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((config.totalSteps)); ++radialIndex)
    {
      size_t baseIndexInner = xIndex * ((config.totalSteps) - 1) + radialIndex + indexOffset;
      _faces.push_back(new Face{_vertices[baseIndexInner],
                                _vertices[baseIndexInner + 1],
                                _vertices[baseIndexInner + (config.totalSteps)],
                                _vertices[baseIndexInner + (config.totalSteps) - 1]});
    }
    if (quadrant == 0 || quadrant == 3)
    {
      size_t baseIndex      = xIndex * indexIncrement + connectingIndex;
      size_t baseIndexInner = xIndex * ((config.totalSteps) - 1) + indexOffset;
      _faces.push_back(new Face{_vertices[baseIndex],
                                _vertices[baseIndexInner],
                                _vertices[baseIndexInner + (config.totalSteps) - 1],
                                _vertices[baseIndex + indexIncrement]});
    }
    else if (quadrant == 1 || quadrant == 2)
    {
      size_t baseIndex      = connectingIndex - xIndex * indexIncrement;
      size_t baseIndexInner = xIndex * ((config.totalSteps) - 1) + indexOffset;
      _faces.push_back(new Face{_vertices[baseIndexInner],
                                _vertices[baseIndex + indexIncrement],
                                _vertices[baseIndex],
                                _vertices[baseIndexInner + (config.totalSteps) - 1]});
    }
  }
  if (quadrant < 3)
  {
    if (quadrant == 0)
    {
      for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((config.totalSteps)); ++radialIndex)
      {
        size_t baseIndexInner = radialIndex + indexOffset;
        _faces.push_back(
            new Face{_vertices[baseIndexInner],
                     _vertices[baseIndexInner + 1],
                     _vertices[baseIndexInner + (config.innerBlockCount - 1) * ((config.totalSteps) - 1) +
                               3 * (config.innerBlockCount) * ((config.totalSteps) - 1) + 1],
                     _vertices[baseIndexInner + (config.innerBlockCount - 1) * ((config.totalSteps) - 1) +
                               3 * (config.innerBlockCount) * ((config.totalSteps) - 1)]});
      }
    }
    for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((config.totalSteps)); ++radialIndex)
    {
      long   xIndex         = static_cast<long>(config.innerBlockCount) - 1;
      size_t baseIndexInner = xIndex * ((config.totalSteps) - 1) + radialIndex + indexOffset;
      _faces.push_back(new Face{_vertices[baseIndexInner],
                                _vertices[baseIndexInner + 1],
                                _vertices[baseIndexInner + (config.totalSteps)],
                                _vertices[baseIndexInner + (config.totalSteps) - 1]});
    }
  }
}
void Mesh2D::buildMesh(Config &config)
{
  buildInnerSquare(config);
  auto   indexIncrements = std::vector<size_t>{config.innerBlockCount + 1, 1, config.innerBlockCount + 1, 1};
  auto   connectingIndices = std::vector<size_t>{config.innerBlockCount,
                                               config.innerBlockCount * (config.innerBlockCount + 2) - 1,
                                               (config.innerBlockCount - 1) * (config.innerBlockCount + 1),
                                               0};
  size_t indexOffset       = (config.innerBlockCount + 1) * (config.innerBlockCount + 1);
  for (size_t quadrant = 0; quadrant != 4; ++quadrant)
  {
    buildCircleQuadrant(config, quadrant);
    indexOffset += (config.innerBlockCount + 1) * (config.totalSteps - 1);
  }
  indexOffset = (config.innerBlockCount + 1) * (config.innerBlockCount + 1);
  for (size_t quadrant = 0; quadrant != 4; ++quadrant)
  {
    buildQuadrantFaces(config, indexOffset, indexIncrements[quadrant], connectingIndices[quadrant], quadrant);
    indexOffset += config.innerBlockCount * (config.totalSteps - 1);
  }
}