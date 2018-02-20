#include "Mesh2DBuilder.h"
#include <climits>
#include <cmath>

template <typename T>
T sq(const T &val) {
  return val * val;
}

void buildInnerSquare(const BaseConfig &baseConfig, Mesh2D &mesh) {
  for (long yIndex = -static_cast<long>(baseConfig.halfInnerBlockCount);
       yIndex != static_cast<long>(baseConfig.halfInnerBlockCount) + 1;
       ++yIndex) {
    double absX = std::abs(yIndex) / static_cast<double>(baseConfig.halfInnerBlockCount);
    for (long xIndex = -static_cast<long>(baseConfig.halfInnerBlockCount);
         xIndex != static_cast<long>(baseConfig.halfInnerBlockCount) + 1;
         ++xIndex) {
      double absY = std::abs(xIndex) / static_cast<double>(baseConfig.halfInnerBlockCount);
      mesh.addVertex((1.0 - baseConfig.alpha * sq(absX)) * xIndex * baseConfig.innerBlockWidth,
                     (1.0 - baseConfig.alpha * sq(absY)) * yIndex * baseConfig.innerBlockWidth);
    }
  }

  for (size_t yIndex = 0; yIndex != baseConfig.innerBlockCount; ++yIndex) {
    for (size_t xIndex = 0; xIndex != baseConfig.innerBlockCount; ++xIndex) {
      size_t baseIndex = xIndex * (baseConfig.innerBlockCount + 1) + yIndex;
      mesh.addFace(baseIndex,
                   baseIndex + 1,
                   baseIndex + baseConfig.innerBlockCount + 2,
                   baseIndex + baseConfig.innerBlockCount + 1,
                   Face2D::FaceType::Nozzle);
    }
  }
}

typedef std::pair<double, double> doublePair;

doublePair operator*(const double m, doublePair const &c) {
  doublePair r(c);
  r.first *= m;
  r.second *= m;
  return r;
}

doublePair operator+(const doublePair &lhs, const doublePair &rhs) {
  doublePair r(rhs);
  r.first += lhs.first;
  r.second += lhs.second;
  return r;
}

doublePair swapCoordsToQuadrant(size_t quadrant, const doublePair &coords) {
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
  return std::make_pair(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
}

struct Square {
  static doublePair getCoords(const size_t index, const size_t numBlocks, const double radius) {
    return std::make_pair(radius * 2. * (index - numBlocks / 2.) / static_cast<double>(numBlocks), -radius);
  }
};

struct Circle {
  static doublePair getCoords(const size_t index, const size_t numBlocks, const double radius) {
    const double phi = M_PI * 0.5 * (index - numBlocks / 2.) / static_cast<double>(numBlocks);
    return std::make_pair(radius * std::sin(phi), -radius * std::cos(phi));
  }
};

template <typename C1, typename C2>
struct Sqircle {
  Sqircle(double blend = 0) : _blend(blend) {}

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
  double                 x;
  double                 y;
  std::array<size_t, 4U> newVertexIndices;

  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    newVertexIndices[quadrant] = mesh.getVertices().size();
    for (size_t i = 0; i != newBlockCount; ++i) {
      std::tie(x, y) = swapCoordsToQuadrant(quadrant, coordinates.getCoords(i, newBlockCount, radius));
      mesh.addVertex(x, y);
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

  for (size_t q = 0; q != 4; ++q) {
    startIndices[q]    = newVertexIndices[q];
    indexIncrements[q] = 1;
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
  double x;
  double y;

  std::array<size_t, 4U> newVertexIndices;
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    newVertexIndices[quadrant] = mesh.getVertices().size();
    for (size_t i = 0; i <= newBlockCount + 1; ++i) {
      std::tie(x, y) = swapCoordsToQuadrant(quadrant, coordinates.getCoords(i, newBlockCount + 2, radius));
      mesh.addVertex(x, y);
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

  for (size_t q = 0; q != 4; ++q) {
    startIndices[q]    = newVertexIndices[q];
    indexIncrements[q] = 1;
  }
  newBlockCount += 2;
}

void buildMesh(const BaseConfig &baseConfig, Mesh2D &mesh) {
  buildInnerSquare(baseConfig, mesh);
  std::array<size_t, 4U> startIndices = {
      0U,
      baseConfig.innerBlockCount,
      (1U + baseConfig.innerBlockCount) * (1U + baseConfig.innerBlockCount) - 1U,
      (baseConfig.innerBlockCount) * (1U + baseConfig.innerBlockCount)};

  std::array<long, 4U> indexIncrements = {1L,
                                          static_cast<long>(baseConfig.innerBlockCount) + 1L,
                                          -1L,
                                          -static_cast<long>(baseConfig.innerBlockCount) - 1L};

  size_t blockCount = baseConfig.innerBlockCount;
  double radius     = (blockCount + 2.) * baseConfig.innerBlockWidth / 2.;

  const size_t squareToNozzleSteps = 2;
  const size_t nozzleToSquareSteps = 3;
  const size_t squareToPipeSteps   = 3;
  const size_t pipeToSquareSteps   = 3;

  // Initial ring
  for (size_t i = 0; i != squareToNozzleSteps; ++i) {
    addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Face2D::FaceType::Nozzle, Circle());
    radius *= (blockCount + 2) / static_cast<double>(blockCount);
  }

  // Transition to regular rectangular
  Sqircle<Circle, Square> sqircle(1.);
  for (size_t i = 0; i != nozzleToSquareSteps; ++i) {
    sqircle._blend -= 1. / nozzleToSquareSteps;
    addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Face2D::FaceType::Pipe, sqircle);
    radius *= (blockCount + 2) / static_cast<double>(blockCount);
  }

  // Regular Rectangular
  sqircle._blend = 0;
  for (size_t i = 0; i != squareToPipeSteps; ++i) {
    addRing(mesh, radius, startIndices, indexIncrements, blockCount, Face2D::FaceType::Pipe, sqircle);
    sqircle._blend += 0.7 * 1. / squareToPipeSteps;

    radius *= (blockCount + 2) / static_cast<double>(blockCount);
  }

  const double dBlend = (1. - sqircle._blend) / pipeToSquareSteps;

  for (size_t i = 0; i != pipeToSquareSteps; ++i) {
    addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Face2D::FaceType::Pipe, sqircle);
    sqircle._blend += dBlend;

    radius *= (blockCount + 2) / static_cast<double>(blockCount);
  }

  for (size_t i = 0; i != 1; ++i) {
    addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Face2D::FaceType::Pipe, Circle());
    radius *= (blockCount + 2) / static_cast<double>(blockCount);
  }

  for (size_t i = 0; i != 1; ++i) {
    addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Face2D::FaceType::Outer, sqircle);
    sqircle._blend -= 1. / 5.;

    radius *= (blockCount + 2) / static_cast<double>(blockCount);
  }
}

Mesh2D Mesh2DBuilder::buildAirBearingMesh2D() {
  Mesh2D     mesh;
  BaseConfig baseConfig;
  buildMesh(baseConfig, mesh);
  return mesh;
}