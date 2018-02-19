#include "Mesh2DBuilder.h"
#include "CoordinateHelper.h"
#include <climits>
#include <cmath>

template <typename T>
T sq(const T &val)
{
  return val * val;
}

void buildInnerSquare(BaseConfig &baseConfig, Mesh2D &mesh)
{
  auto &vertices = mesh.getVertices();
  auto &faces    = mesh.getFaces();
  for (long yIndex = -static_cast<long>(baseConfig.halfInnerBlockCount);
       yIndex != static_cast<long>(baseConfig.halfInnerBlockCount) + 1;
       ++yIndex) {
    double absX = std::abs(yIndex) / static_cast<double>(baseConfig.halfInnerBlockCount);
    for (long xIndex = -static_cast<long>(baseConfig.halfInnerBlockCount);
         xIndex != static_cast<long>(baseConfig.halfInnerBlockCount) + 1;
         ++xIndex) {
      double absY = std::abs(xIndex) / static_cast<double>(baseConfig.halfInnerBlockCount);
      vertices.push_back(new Vertex(
                           sf::Vector2f((1.0 - baseConfig.alpha * sq(absX)) * xIndex * baseConfig.innerBlockWidth,
                                        (1.0 - baseConfig.alpha * sq(absY)) * yIndex * baseConfig.innerBlockWidth)));
    }
  }
  for (size_t yIndex = 0; yIndex != baseConfig.innerBlockCount; ++yIndex) {
    for (size_t xIndex = 0; xIndex != baseConfig.innerBlockCount; ++xIndex) {
      size_t baseIndex = xIndex * (baseConfig.innerBlockCount + 1) + yIndex;
      faces.push_back(new Face{vertices[baseIndex],
                               vertices[baseIndex + 1],
                               vertices[baseIndex + baseConfig.innerBlockCount + 2],
                               vertices[baseIndex + baseConfig.innerBlockCount + 1]});
    }
  }
}

void buildOuterRing(BaseConfig &baseConfig, Mesh2D &mesh)
{
  auto &                         vertices = mesh.getVertices();
  CoordinateHelper               coordinateHelper(baseConfig);
  const CoordinateHelper::Region regions[3] = {
    CoordinateHelper::Nozzle, CoordinateHelper::Pipe, CoordinateHelper::Exterior
  };
  const size_t blockCounts[3] = {baseConfig.nozzleSteps, baseConfig.pipeSteps, baseConfig.outerSteps};
  double       x;
  double       y;
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    for (long yIndex = -static_cast<long>(baseConfig.halfInnerBlockCount);
         yIndex != static_cast<long>(baseConfig.halfInnerBlockCount);
         ++yIndex) {
      coordinateHelper.yIndexChanged(yIndex);
      for (size_t regionIdx = 0; regionIdx != 3; ++regionIdx) {
        for (size_t radialIndex = 0; radialIndex != blockCounts[regionIdx]; ++radialIndex) {
          std::tie(x, y) = coordinateHelper.getCoords(radialIndex, regions[regionIdx], quadrant);
          vertices.push_back(new Vertex(sf::Vector2f(x, y)));
        }
      }
    }
  }
}

void buildSquareQuadrantVertices(BaseConfig &baseConfig, size_t quadrant, Mesh2D &mesh)
{
  auto &vertices = mesh.getVertices();
  for (long yIndex = -static_cast<long>(baseConfig.halfInnerBlockCount);
       yIndex != static_cast<long>(baseConfig.halfInnerBlockCount);
       ++yIndex) {
    for (long xIndex = 1; xIndex != static_cast<long>(baseConfig.totalSteps); ++xIndex) {
      double y = yIndex;
      double x = 0.5 * baseConfig.innerSquareSide + xIndex * baseConfig.innerBlockWidth;
      for (size_t q = 0; q != quadrant; ++q) {
        std::tie(x, y) = std::make_pair(-y, x);
      }
      vertices.push_back(new Vertex(sf::Vector2f(x, y)));
    }
  }
}
void buildQuadrantFaces( // The horror
  BaseConfig &baseConfig,
  size_t      indexOffset,
  long        indexIncrement,
  size_t      connectingIndex,
  size_t      quadrant,
  Mesh2D &    mesh)
{
  auto &vertices = mesh.getVertices();
  auto &faces    = mesh.getFaces();
  for (long yIndex = 0; yIndex != static_cast<long>(baseConfig.innerBlockCount) - 1; ++yIndex) {
    for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((baseConfig.totalSteps)); ++radialIndex) {
      size_t baseIndexInner = yIndex * ((baseConfig.totalSteps) - 1) + radialIndex + indexOffset;
      faces.push_back(new Face{vertices[baseIndexInner],
                               vertices[baseIndexInner + 1],
                               vertices[baseIndexInner + (baseConfig.totalSteps)],
                               vertices[baseIndexInner + (baseConfig.totalSteps) - 1]});
    }
    if (quadrant == 0 || quadrant == 3) {
      size_t baseIndex      = yIndex * indexIncrement + connectingIndex;
      size_t baseIndexInner = yIndex * ((baseConfig.totalSteps) - 1) + indexOffset;
      faces.push_back(new Face{vertices[baseIndex],
                               vertices[baseIndexInner],
                               vertices[baseIndexInner + (baseConfig.totalSteps) - 1],
                               vertices[baseIndex + indexIncrement]});
    } else if (quadrant == 1 || quadrant == 2) {
      size_t baseIndex      = connectingIndex - yIndex * indexIncrement;
      size_t baseIndexInner = yIndex * ((baseConfig.totalSteps) - 1) + indexOffset;
      faces.push_back(new Face{vertices[baseIndexInner],
                               vertices[baseIndex + indexIncrement],
                               vertices[baseIndex],
                               vertices[baseIndexInner + (baseConfig.totalSteps) - 1]});
    }
  }
  if (quadrant < 3) {
    if (quadrant == 0) {
      for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((baseConfig.totalSteps)); ++radialIndex) {
        size_t baseIndexInner = radialIndex + indexOffset;
        faces.push_back(new Face{
          vertices[baseIndexInner],
          vertices[baseIndexInner + 1],
          vertices[baseIndexInner + (baseConfig.innerBlockCount - 1) * ((baseConfig.totalSteps) - 1) +
          3 * (baseConfig.innerBlockCount) * ((baseConfig.totalSteps) - 1) + 1],
          vertices[baseIndexInner + (baseConfig.innerBlockCount - 1) * ((baseConfig.totalSteps) - 1) +
          3 * (baseConfig.innerBlockCount) * ((baseConfig.totalSteps) - 1)]});
      }
    }
    for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((baseConfig.totalSteps)); ++radialIndex) {
      long   yIndex         = static_cast<long>(baseConfig.innerBlockCount) - 1;
      size_t baseIndexInner = yIndex * ((baseConfig.totalSteps) - 1) + radialIndex + indexOffset;
      faces.push_back(new Face{vertices[baseIndexInner],
                               vertices[baseIndexInner + 1],
                               vertices[baseIndexInner + (baseConfig.totalSteps)],
                               vertices[baseIndexInner + (baseConfig.totalSteps) - 1]});
    }
  }
}

void connectVertices(BaseConfig &baseConfig, Mesh2D &mesh)
{
  auto indexIncrements =
    std::vector<size_t> {baseConfig.innerBlockCount + 1, 1, baseConfig.innerBlockCount + 1, 1};
  auto connectingIndices =
    std::vector<size_t> {baseConfig.innerBlockCount,
                         baseConfig.innerBlockCount * (baseConfig.innerBlockCount + 2) - 1,
                         (baseConfig.innerBlockCount - 1) * (baseConfig.innerBlockCount + 1),
                         0
                        };
  size_t indexOffset = (baseConfig.innerBlockCount + 1) * (baseConfig.innerBlockCount + 1);
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    buildQuadrantFaces(
      baseConfig, indexOffset, indexIncrements[quadrant], connectingIndices[quadrant], quadrant, mesh);
    indexOffset += baseConfig.innerBlockCount * (baseConfig.totalSteps - 1);
  }
}

void buildMesh(BaseConfig &baseConfig, Mesh2D &mesh)
{
  buildInnerSquare(baseConfig, mesh);
  buildOuterRing(baseConfig, mesh);
  connectVertices(baseConfig, mesh);
}

Mesh2D Mesh2DBuilder::buildAirBearingMesh2D()
{
  Mesh2D     mesh;
  BaseConfig baseConfig;
  buildMesh(baseConfig, mesh);
  std::cout << mesh.getFaces().size() << " faces and " << mesh.getVertices().size() << " vertices \n";
  return mesh;
}