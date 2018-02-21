#include "Mesh2DBuilder.h"
#include <climits>
#include <cmath>
#include "BaseConfig.h"

template <typename T>
T sq(const T &val) {
  return val * val;
}

void buildInnerSquare(const BaseConfig &baseConfig, Mesh2D &mesh) {
  const long halfInnerBlockCount = baseConfig._innerBlockCount / 2;
  for (long yIndex = -halfInnerBlockCount; yIndex != halfInnerBlockCount + 1; ++yIndex) {
    double absX = std::abs(yIndex) / halfInnerBlockCount;
    for (long xIndex = -halfInnerBlockCount; xIndex != halfInnerBlockCount + 1; ++xIndex) {
      double absY = std::abs(xIndex) / halfInnerBlockCount;
      mesh.addVertex((1.0 - baseConfig._alpha * sq(absX)) * xIndex * baseConfig._innerBlockWidth,
                     (1.0 - baseConfig._alpha * sq(absY)) * yIndex * baseConfig._innerBlockWidth);
    }
  }
  const size_t innerBlockCount = baseConfig._innerBlockCount;

  for (size_t yIndex = 0; yIndex != innerBlockCount; ++yIndex) {
    for (size_t xIndex = 0; xIndex != innerBlockCount; ++xIndex) {
      size_t baseIndex = xIndex * (innerBlockCount + 1) + yIndex;
      mesh.addFace(baseIndex,
                   baseIndex + 1,
                   baseIndex + innerBlockCount + 2,
                   baseIndex + innerBlockCount + 1,
                   Face2D::FaceType::Nozzle);
    }
  }
}

std::pair<double, double> swapCoordsToQuadrant(size_t quadrant, const doublePair &coords) {
  switch (quadrant) {
    case 0:
      return std::make_pair(coords.first, coords.second);
    case 1:
      return std::make_pair(-coords.second, coords.first);
    case 2:
      return std::make_pair(-coords.first, -coords.second);
    case 3:
      return std::make_pair(coords.second, -coords.first);
  }
  return std::make_pair(0., 0.);
}

struct Square {
  constexpr static doublePair getCoords(const size_t index, const size_t numBlocks, const double radius) {
    return std::make_pair(radius * 2. * (index - numBlocks / 2.) / numBlocks, -radius);
  }
};

struct Circle {
  static doublePair getCoords(const size_t index, const size_t numBlocks, const double radius) {
    const double phi = M_PI * 0.5 * (index - numBlocks / 2.) / numBlocks;
    return std::make_pair(radius * std::sin(phi), -radius * std::cos(phi));
  }
};

template <typename C1, typename C2>
struct Sqircle {
  Sqircle(double blend = 0.) : _blend(blend) {}

  doublePair getCoords(const size_t index, const size_t numBlocks, const double radius) const {
    return _blend * C1::getCoords(index, numBlocks, radius) +
           (1. - _blend) * C2::getCoords(index, numBlocks, radius);
  }

  double _blend;
};

template <typename C>
void addSkipRing(Mesh2D &                mesh,
                 const double            radius,
                 std::array<size_t, 4U> &startIndices,
                 std::array<long, 4U> &  indexIncrements,
                 const size_t            newBlockCount,
                 const Face2D::FaceType  type,
                 const C &               coordinates) {
  std::array<size_t, 4U> newVertexIndices;

  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    newVertexIndices[quadrant] = mesh.getVertices().size();
    for (size_t i = 0; i != newBlockCount; ++i) {
      mesh.addVertex(swapCoordsToQuadrant(quadrant, coordinates.getCoords(i, newBlockCount, radius)));
    }
  }
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    for (size_t i = 0; i + 1 != newBlockCount; ++i) {
      mesh.addFace(startIndices[quadrant] + i * indexIncrements[quadrant],
                   startIndices[quadrant] + (i + 1) * indexIncrements[quadrant],
                   newVertexIndices[quadrant] + 1 + i,
                   newVertexIndices[quadrant] + i,
                   type);
    }
    mesh.addFace(startIndices[quadrant] + (newBlockCount - 1) * indexIncrements[quadrant],
                 startIndices[(quadrant + 1) % 4],
                 newVertexIndices[(quadrant + 1) % 4],
                 newVertexIndices[quadrant] + newBlockCount - 1,
                 type);
  }
}

template <typename C>
void addRing(Mesh2D &                mesh,
             const double            radius,
             std::array<size_t, 4U> &startIndices,
             std::array<long, 4U> &  indexIncrements,
             size_t &                newBlockCount,
             const Face2D::FaceType  type,
             const C &               coordinates) {
  std::array<size_t, 4U> newVertexIndices;
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    newVertexIndices[quadrant] = mesh.getVertices().size();
    for (size_t i = 0; i <= newBlockCount + 1; ++i) {
      mesh.addVertex(swapCoordsToQuadrant(quadrant, coordinates.getCoords(i, newBlockCount + 2, radius)));
    }
  }
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    for (size_t i = 0; i != newBlockCount; ++i) {
      if (quadrant == 3 && i + 1 == newBlockCount) {
        mesh.addFace(startIndices[quadrant] + i * indexIncrements[quadrant],
                     startIndices[0],
                     newVertexIndices[quadrant] + 1 + i + 1,
                     newVertexIndices[quadrant] + i + 1,
                     type);
      } else {
        mesh.addFace(startIndices[quadrant] + i * indexIncrements[quadrant],
                     startIndices[quadrant] + (i + 1) * indexIncrements[quadrant],
                     newVertexIndices[quadrant] + 1 + i + 1,
                     newVertexIndices[quadrant] + i + 1,
                     type);
      }
    }

    mesh.addFace(
        newVertexIndices[quadrant],
        newVertexIndices[quadrant] + 1,
        startIndices[quadrant],
        newVertexIndices[(quadrant - 1) % 4] + (newBlockCount + 1) * indexIncrements[(quadrant - 1) % 4],
        type);
  }
  newBlockCount += 2;
}

template <typename C>
void addSkipRing(Mesh2D &               mesh,
                 const double           radius,
                 size_t &               newBlockCount,
                 const Face2D::FaceType type,
                 const C &              coordinates) {
  std::array<size_t, 4U> startIndices    = {mesh.getVertices().size() - 4 * newBlockCount,
                                         mesh.getVertices().size() - 3 * newBlockCount,
                                         mesh.getVertices().size() - 2 * newBlockCount,
                                         mesh.getVertices().size() - 1 * newBlockCount};
  std::array<long, 4U>   indexIncrements = {1L, 1L, 1L, 1L};
  addSkipRing(mesh, radius, startIndices, indexIncrements, newBlockCount, type, coordinates);
}

template <typename C>
void addRing(Mesh2D &               mesh,
             const double           radius,
             size_t &               newBlockCount,
             const Face2D::FaceType type,
             const C &              coordinates) {
  std::array<size_t, 4U> startIndices    = {mesh.getVertices().size() - 4 * newBlockCount,
                                         mesh.getVertices().size() - 3 * newBlockCount,
                                         mesh.getVertices().size() - 2 * newBlockCount,
                                         mesh.getVertices().size() - 1 * newBlockCount};
  std::array<long, 4U>   indexIncrements = {1L, 1L, 1L, 1L};
  addRing(mesh, radius, startIndices, indexIncrements, newBlockCount, type, coordinates);
}

void buildNozzle(const BaseConfig &baseConfig, Mesh2D &mesh, size_t &blockCount, double &radius) {
  std::array<size_t, 4U> startIndices = {
      0U,
      baseConfig._innerBlockCount,
      (1U + baseConfig._innerBlockCount) * (1U + baseConfig._innerBlockCount) - 1U,
      (baseConfig._innerBlockCount) * (1U + baseConfig._innerBlockCount)};

  std::array<long, 4U> indexIncrements = {1L,
                                          static_cast<long>(baseConfig._innerBlockCount) + 1L,
                                          -1L,
                                          -static_cast<long>(baseConfig._innerBlockCount) - 1L};

  const size_t squareToNozzleSteps = 3;

  double c =
      std::pow(baseConfig._nozzleRadius / baseConfig._innerSquareWidth, 1. / (squareToNozzleSteps + 1.));

  addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Face2D::FaceType::Nozzle, Circle());
  radius *= c;

  // Initial ring
  for (size_t i = 1; i != squareToNozzleSteps; ++i) {
    addSkipRing(mesh, radius, blockCount, Face2D::FaceType::Nozzle, Circle());
    radius *= c;
  }
}

void buildPipe(const BaseConfig &baseConfig, Mesh2D &mesh, size_t &blockCount, double &radius) {
  const size_t nozzleToSquareSteps   = 4;
  const size_t regularRectangleSteps = 2;
  const size_t regularBendSteps      = 2;
  const size_t squareToPipeSteps     = 3;

  // Transition to regular rectangular
  Sqircle<Circle, Square> sqircle(1.);
  for (size_t i = 0; i != nozzleToSquareSteps; ++i) {
    sqircle._blend -= 0.6 / (nozzleToSquareSteps);
    addSkipRing(mesh, radius, blockCount, Face2D::FaceType::Pipe, sqircle);
    radius *= 1.15;
  }

  // Regular Rectangular
  sqircle._blend = 0.15;
  for (size_t i = 0; i != regularRectangleSteps; ++i) {
    addRing(mesh, radius, blockCount, Face2D::FaceType::Pipe, sqircle);
    radius *= (blockCount + 2.) / blockCount;
  }

  sqircle._blend = 0.15;
  for (size_t i = 0; i != regularBendSteps; ++i) {
    addRing(mesh, radius, blockCount, Face2D::FaceType::Pipe, sqircle);
    sqircle._blend += .85 / (regularBendSteps + squareToPipeSteps);
    radius *= (blockCount + 2.) / blockCount;
  }

  const double dBlend = (1. - sqircle._blend) / (squareToPipeSteps);

  for (size_t i = 0; i != squareToPipeSteps + 1; ++i) {
    addSkipRing(mesh, radius, blockCount, Face2D::FaceType::Pipe, sqircle);
    sqircle._blend += dBlend;
    radius *= (blockCount + 2.) / blockCount;
  }
}

void buildMesh(const BaseConfig &baseConfig, Mesh2D &mesh) {
  buildInnerSquare(baseConfig, mesh);
  size_t blockCount = baseConfig._innerBlockCount;
  double radius     = (blockCount + 2.) * baseConfig._innerBlockWidth / 2.;
  buildNozzle(baseConfig, mesh, blockCount, radius);
  buildPipe(baseConfig, mesh, blockCount, radius);
}

Mesh2D Mesh2DBuilder::buildAirBearingMesh2D() {
  Mesh2D     mesh;
  BaseConfig baseConfig;
  buildMesh(baseConfig, mesh);
  return mesh;
}