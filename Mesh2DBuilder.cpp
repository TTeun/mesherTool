#include "Mesh2DBuilder.h"
#include <climits>
#include <cmath>
#include "CoordinateHelper.h"

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
                   baseIndex + baseConfig.innerBlockCount + 1);
    }
  }
}

void buildOuterRing(const BaseConfig &baseConfig, Mesh2D &mesh) {
  CoordinateHelper coordinateHelper(baseConfig);

  const CoordinateHelper::Region regions[3] = {
      CoordinateHelper::Nozzle, CoordinateHelper::Pipe, CoordinateHelper::Exterior};
  const size_t blockCounts[3] = {baseConfig.nozzleSteps, baseConfig.pipeSteps, baseConfig.outerSteps};

  double x;
  double y;
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    for (long yIndex = -static_cast<long>(baseConfig.halfInnerBlockCount);
         yIndex != static_cast<long>(baseConfig.halfInnerBlockCount);
         ++yIndex) {
      coordinateHelper.yIndexChanged(yIndex);
      for (size_t regionIdx = 0; regionIdx != 3; ++regionIdx) {
        for (size_t radialIndex = 0; radialIndex != blockCounts[regionIdx]; ++radialIndex) {
          std::tie(x, y) = coordinateHelper.getCoords(radialIndex, regions[regionIdx], quadrant);
          mesh.addVertex(x, y);
        }
      }
    }
  }
}

void buildSquareQuadrantVertices(const BaseConfig &baseConfig, size_t quadrant, Mesh2D &mesh) {
  for (long yIndex = -static_cast<long>(baseConfig.halfInnerBlockCount);
       yIndex != static_cast<long>(baseConfig.halfInnerBlockCount);
       ++yIndex) {
    for (long xIndex = 1; xIndex != static_cast<long>(baseConfig.totalSteps); ++xIndex) {
      double y = yIndex;
      double x = 0.5 * baseConfig.innerSquareSide + xIndex * baseConfig.innerBlockWidth;
      for (size_t q = 0; q != quadrant; ++q) {
        std::tie(x, y) = std::make_pair(-y, x);
      }
      mesh.addVertex(x, y);
    }
  }
}

void buildQuadrantFaces(  // The horror
    const BaseConfig &baseConfig,
    size_t            indexOffset,
    long              indexIncrement,
    size_t            connectingIndex,
    size_t            quadrant,
    Mesh2D &          mesh) {
  for (long yIndex = 0; yIndex != static_cast<long>(baseConfig.innerBlockCount) - 1; ++yIndex) {
    for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((baseConfig.totalSteps)); ++radialIndex) {
      size_t baseIndexInner = yIndex * ((baseConfig.totalSteps) - 1) + radialIndex + indexOffset;
      mesh.addFace(baseIndexInner,
                   baseIndexInner + 1,
                   baseIndexInner + (baseConfig.totalSteps),
                   baseIndexInner + (baseConfig.totalSteps) - 1);
    }

    if (quadrant == 0 || quadrant == 3) {
      size_t baseIndex      = yIndex * indexIncrement + connectingIndex;
      size_t baseIndexInner = yIndex * ((baseConfig.totalSteps) - 1) + indexOffset;
      mesh.addFace(baseIndex,
                   baseIndexInner,
                   baseIndexInner + (baseConfig.totalSteps) - 1,
                   baseIndex + indexIncrement);
    } else if (quadrant == 1 || quadrant == 2) {
      size_t baseIndex      = connectingIndex - yIndex * indexIncrement;
      size_t baseIndexInner = yIndex * ((baseConfig.totalSteps) - 1) + indexOffset;
      mesh.addFace(baseIndexInner,
                   baseIndex + indexIncrement,
                   baseIndex,
                   baseIndexInner + (baseConfig.totalSteps) - 1);
    }
  }

  if (quadrant < 3) {
    if (quadrant == 0) {
      for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((baseConfig.totalSteps));
           ++radialIndex) {
        size_t baseIndexInner = radialIndex + indexOffset;
        mesh.addFace(baseIndexInner,
                     baseIndexInner + 1,
                     baseIndexInner + (baseConfig.innerBlockCount - 1) * ((baseConfig.totalSteps) - 1) +
                         3 * (baseConfig.innerBlockCount) * ((baseConfig.totalSteps) - 1) + 1,
                     baseIndexInner + (baseConfig.innerBlockCount - 1) * ((baseConfig.totalSteps) - 1) +
                         3 * (baseConfig.innerBlockCount) * ((baseConfig.totalSteps) - 1));
      }
    }
    for (long radialIndex = 0; radialIndex + 2 != static_cast<long>((baseConfig.totalSteps)); ++radialIndex) {
      long   yIndex         = static_cast<long>(baseConfig.innerBlockCount) - 1;
      size_t baseIndexInner = yIndex * ((baseConfig.totalSteps) - 1) + radialIndex + indexOffset;
      mesh.addFace(baseIndexInner,
                   baseIndexInner + 1,
                   baseIndexInner + (baseConfig.totalSteps),
                   baseIndexInner + (baseConfig.totalSteps) - 1);
    }
  }
}

void connectVertices(const BaseConfig &baseConfig, Mesh2D &mesh) {
  auto indexIncrements =
      std::vector<size_t>{baseConfig.innerBlockCount + 1, 1, baseConfig.innerBlockCount + 1, 1};
  auto connectingIndices =
      std::vector<size_t>{baseConfig.innerBlockCount,
                          baseConfig.innerBlockCount * (baseConfig.innerBlockCount + 2) - 1,
                          (baseConfig.innerBlockCount - 1) * (baseConfig.innerBlockCount + 1),
                          0};
  size_t indexOffset = (baseConfig.innerBlockCount + 1) * (baseConfig.innerBlockCount + 1);
  for (size_t quadrant = 0; quadrant != 4; ++quadrant) {
    buildQuadrantFaces(
        baseConfig, indexOffset, indexIncrements[quadrant], connectingIndices[quadrant], quadrant, mesh);
    indexOffset += baseConfig.innerBlockCount * (baseConfig.totalSteps - 1);
  }
}

std::pair<double, double> swapCoordsToQuadrant(size_t quadrant, double x, double y) {
  switch (quadrant) {
    case 0:
      return std::make_pair(x, y);
    case 1:
      return std::make_pair(-y, x);
    case 2:
      return std::make_pair(-x, -y);
    case 3:
      return std::make_pair(y, -x);
  }
  return std::make_pair(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
}

std::pair<double, double> swapCoordsToQuadrant(size_t quadrant, const std::pair<double, double> &coords) {
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
  static std::pair<double, double> getCoords(const size_t index,
                                             const size_t numBlocks,
                                             const double radius) {
    return std::make_pair(radius * 2. * (index - numBlocks / 2.) / static_cast<double>(numBlocks), -radius);
  }
};

struct Circle {
  static std::pair<double, double> getCoords(const size_t index,
                                             const size_t numBlocks,
                                             const double radius) {
    const double phi = M_PI * 0.5 * (index - numBlocks / 2.) / static_cast<double>(numBlocks);
    return std::make_pair(radius * std::sin(phi), -radius * std::cos(phi));
  }
};

std::pair<double, double> operator*(const double m, std::pair<double, double> const &c) {
  std::pair<double, double> r(c);
  r.first *= m;
  r.second *= m;
  return r;
}

std::pair<double, double> operator+(std::pair<double, double> const &lhs,
                                    std::pair<double, double> const &rhs) {
  std::pair<double, double> r(rhs);
  r.first += lhs.first;
  r.second *= lhs.second;
  return r;
}

// template <typename C1, typename C2>
// struct Sqircle {
//   Sqircle(double radius, const size_t numLayers, double blend = 0.)
//       : _radius(radius), _numLayers(numLayers), _blend(blend), _coords1(radius), _coords2(radius) {}

//   std::pair<double, double> getCoords(const size_t index, const size_t numBlocks) const {
//     _blend = 0;
//     return _blend * C1::getCoords(index, numBlocks, _radius) +
//            (1. - _blend) * C2::getCoords(index, numBlocks, _radius);
//   }
//   void toNextLayer() {
//     _blend += 1. / _numLayers;
//     _radius += 0.1;
//   }

//   double       _radius;
//   const size_t _numLayers;
//   double       _blend;
//   C1           _coords1;
//   C2           _coords2;
// };

template <typename C>
void addSkipRing(Mesh2D &                mesh,
                 const double            radius,
                 std::array<size_t, 4U> &startIndices,
                 std::array<long, 4U> &  indexIncrements,
                 const size_t            newBlockCount,
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
                   newVertexIndices[quadrant] + i);
    }
    mesh.addFace(startIndices[quadrant] + (newBlockCount - 1) * indexIncrements[quadrant],
                 startIndices[(quadrant + 1) % 4],
                 newVertexIndices[(quadrant + 1) % 4],
                 newVertexIndices[quadrant] + newBlockCount - 1);
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
                     newVertexIndices[quadrant] + i + 1);
      } else {
        mesh.addFace(startIndices[quadrant] + i * indexIncrements[quadrant],
                     startIndices[quadrant] + (i + 1) * indexIncrements[quadrant],
                     newVertexIndices[quadrant] + 1 + i + 1,
                     newVertexIndices[quadrant] + i + 1);
      }
    }

    mesh.addFace(
        newVertexIndices[quadrant],
        newVertexIndices[quadrant] + 1,
        startIndices[quadrant],
        newVertexIndices[(quadrant - 1) % 4] + (newBlockCount + 1) * indexIncrements[(quadrant - 1) % 4]);
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
  double radius     = (blockCount + 4.) * baseConfig.innerBlockWidth / 2.;
  // for (size_t l = 0; l != 5; ++l) {
  //   addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  //   radius += 0.002;
  //   // coords.toNextLayer();
  // }
  // addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  // radius += 0.002;
  // addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  // radius += 0.002;
  std::cout << blockCount << '\n';
  // for (size_t l = 0; l != 5; ++l) {
  addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  radius *= (blockCount + 2) / static_cast<double>(blockCount);
  addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  radius *= (blockCount + 2) / static_cast<double>(blockCount);
  addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  radius *= (blockCount + 2) / static_cast<double>(blockCount);

  addRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  radius *= (blockCount + 2) / static_cast<double>(blockCount);
  addRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  radius *= (blockCount + 2) / static_cast<double>(blockCount);
  addRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  radius *= (blockCount + 2) / static_cast<double>(blockCount);

  addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  radius *= (blockCount + 2) / static_cast<double>(blockCount);
  addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  radius *= (blockCount + 2) / static_cast<double>(blockCount);
  addSkipRing(mesh, radius, startIndices, indexIncrements, blockCount, Square());
  radius *= (blockCount + 2) / static_cast<double>(blockCount);

  // buildOuterRing(baseConfig, mesh);

  // connectVertices(baseConfig, mesh);
}

Mesh2D Mesh2DBuilder::buildAirBearingMesh2D() {
  Mesh2D     mesh;
  BaseConfig baseConfig;
  buildMesh(baseConfig, mesh);
  return mesh;
}