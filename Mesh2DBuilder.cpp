#include "Mesh2DBuilder.h"
#include "CoordinateHelper.h"
#include <climits>
#include <cmath>

template <typename T>
T sq(const T &val)
{
  return val * val;
}

void buildInnerSquare(Config &config, Mesh2D &mesh)
{
  auto &vertices = mesh.getVertices();
  auto &faces    = mesh.getFaces();
  for (long yIndex = -static_cast<long>(config.halfInnerBlockCount);
       yIndex != static_cast<long>(config.halfInnerBlockCount) + 1;
       ++yIndex) {
    double absX = std::abs(yIndex) / static_cast<double>(config.halfInnerBlockCount);
    for (long xIndex = -static_cast<long>(config.halfInnerBlockCount);
         xIndex != static_cast<long>(config.halfInnerBlockCount) + 1;
         ++xIndex) {
      double absY = std::abs(xIndex) / static_cast<double>(config.halfInnerBlockCount);
      vertices.push_back(
        new Vertex(sf::Vector2f((1.0 - config.alpha * sq(absX)) * xIndex * config.innerBlockWidth,
                                (1.0 - config.alpha * sq(absY)) * yIndex * config.innerBlockWidth)));
    }
  }
  for (size_t yIndex = 0; yIndex != config.innerBlockCount; ++yIndex) {
    for (size_t xIndex = 0; xIndex != config.innerBlockCount; ++xIndex) {
      size_t baseIndex = xIndex * (config.innerBlockCount + 1) + yIndex;
      faces.push_back(new Face{vertices[baseIndex],
                               vertices[baseIndex + 1],
                               vertices[baseIndex + config.innerBlockCount + 2],
                               vertices[baseIndex + config.innerBlockCount + 1]});
    }
  }
}

void buildCircleQuadrant(Config &config, size_t quadrant, Mesh2D &mesh)
{
  auto &           vertices = mesh.getVertices();
  CoordinateHelper coordinateHelper(config);
  double           x;
  double           y;
  for (long yIndex = -static_cast<long>(config.halfInnerBlockCount);
       yIndex != static_cast<long>(config.halfInnerBlockCount);
       ++yIndex) {
    coordinateHelper.setValues(yIndex);
    for (long radialIndex = 0; radialIndex != static_cast<long>(config.nozzleSteps); ++radialIndex) {
      std::tie(x, y) = coordinateHelper.getCoords(radialIndex, CoordinateHelper::Nozzle, quadrant);
      vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }
    for (long radialIndex = 0; radialIndex != static_cast<long>(config.pipeSteps); ++radialIndex) {
      std::tie(x, y) = coordinateHelper.getCoords(radialIndex, CoordinateHelper::Pipe, quadrant);
      vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }
    for (long radialIndex = 0; radialIndex != static_cast<long>(config.outerSteps); ++radialIndex) {
      std::tie(x, y) = coordinateHelper.getCoords(radialIndex, CoordinateHelper::Exterior, quadrant);
      vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }
  }
}

void buildSquareQuadrantVertices(Config &config, size_t quadrant, Mesh2D &mesh)
{
  auto &vertices = mesh.getVertices();
  for (long yIndex = -static_cast<long>(config.halfInnerBlockCount);
       yIndex != static_cast<long>(config.halfInnerBlockCount);
       ++yIndex) {
    for (long xIndex = 1; xIndex != static_cast<long>(config.totalSteps); ++xIndex) {
      double y = yIndex;
      double x = 0.5 * config.innerSquareSide + xIndex * config.innerBlockWidth;
      for (size_t q = 0; q != quadrant; ++q) {
        std::tie(x, y) = std::make_pair(-y, x);
      }
      vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }
  }
}

void buildQuadrantFaces( // The horror
  Config &config,
  size_t  indexOffset,
  long    indexIncrement,
  size_t  connectingIndex,
  size_t  quadrant,
  Mesh2D &mesh)
{
  auto &vertices = mesh.getVertices();
  auto &faces    = mesh.getFaces();
  for (long yIndex = 0; yIndex != static_cast<long>(config.innerBlockCount) - 1; ++yIndex) {
    for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((config.totalSteps)); ++radialIndex) {
      size_t baseIndexInner = yIndex * ((config.totalSteps) - 1) + radialIndex + indexOffset;
      faces.push_back(new Face{vertices[baseIndexInner],
                               vertices[baseIndexInner + 1],
                               vertices[baseIndexInner + (config.totalSteps)],
                               vertices[baseIndexInner + (config.totalSteps) - 1]});
    }
    if (quadrant == 0 || quadrant == 3) {
      size_t baseIndex      = yIndex * indexIncrement + connectingIndex;
      size_t baseIndexInner = yIndex * ((config.totalSteps) - 1) + indexOffset;
      faces.push_back(new Face{vertices[baseIndex],
                               vertices[baseIndexInner],
                               vertices[baseIndexInner + (config.totalSteps) - 1],
                               vertices[baseIndex + indexIncrement]});
    } else if (quadrant == 1 || quadrant == 2) {
      size_t baseIndex      = connectingIndex - yIndex * indexIncrement;
      size_t baseIndexInner = yIndex * ((config.totalSteps) - 1) + indexOffset;
      faces.push_back(new Face{vertices[baseIndexInner],
                               vertices[baseIndex + indexIncrement],
                               vertices[baseIndex],
                               vertices[baseIndexInner + (config.totalSteps) - 1]});
    }
  }
  if (quadrant < 3) {
    if (quadrant == 0) {
      for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((config.totalSteps)); ++radialIndex) {
        size_t baseIndexInner = radialIndex + indexOffset;
        faces.push_back(
          new Face{vertices[baseIndexInner],
                   vertices[baseIndexInner + 1],
                   vertices[baseIndexInner + (config.innerBlockCount - 1) * ((config.totalSteps) - 1) +
                            3 * (config.innerBlockCount) * ((config.totalSteps) - 1) + 1],
                   vertices[baseIndexInner + (config.innerBlockCount - 1) * ((config.totalSteps) - 1) +
                            3 * (config.innerBlockCount) * ((config.totalSteps) - 1)]});
      }
    }
    for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((config.totalSteps)); ++radialIndex) {
      long   yIndex         = static_cast<long>(config.innerBlockCount) - 1;
      size_t baseIndexInner = yIndex * ((config.totalSteps) - 1) + radialIndex + indexOffset;
      faces.push_back(new Face{vertices[baseIndexInner],
                               vertices[baseIndexInner + 1],
                               vertices[baseIndexInner + (config.totalSteps)],
                               vertices[baseIndexInner + (config.totalSteps) - 1]});
    }
  }
}

void buildMesh(Config &config, Mesh2D &mesh)
{
  buildInnerSquare(config, mesh);
  auto   indexIncrements = std::vector<size_t> {config.innerBlockCount + 1, 1, config.innerBlockCount + 1, 1};
  auto   connectingIndices = std::vector<size_t> {config.innerBlockCount,
         config.innerBlockCount * (config.innerBlockCount + 2) - 1,
         (config.innerBlockCount - 1) * (config.innerBlockCount + 1),
         0
                                                 };
  size_t indexOffset       = (config.innerBlockCount + 1) * (config.innerBlockCount + 1);
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    buildCircleQuadrant(config, quadrant, mesh);
    indexOffset += (config.innerBlockCount + 1) * (config.totalSteps - 1);
  }
  indexOffset = (config.innerBlockCount + 1) * (config.innerBlockCount + 1);
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    buildQuadrantFaces(
      config, indexOffset, indexIncrements[quadrant], connectingIndices[quadrant], quadrant, mesh);
    indexOffset += config.innerBlockCount * (config.totalSteps - 1);
  }
}

Mesh2D Mesh2DBuilder::buildAirBearingMesh2D()
{
  Mesh2D mesh;
  Config config;
  buildMesh(config, mesh);
  std::cout << mesh.getFaces().size() << " faces and " << mesh.getVertices().size() << " vertices \n";
  return mesh;
}