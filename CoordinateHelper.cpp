#include "CoordinateHelper.h"
#include <cmath>
#include <iostream>

std::pair<double, double> CoordinateHelper::getCoords(const long radialIndex,
                                                      Region     region,
                                                      size_t     quadrant) {
  // Initialize x and y to prevent -Wmaybe-uninitialized error
  double x = 0.;
  double y = 0.;
  switch (region) {
    case Nozzle:
      std::tie(x, y) = _nozzleCoordinates.getCoords(radialIndex);
      break;
    case Pipe:
      std::tie(x, y) = _pipeCoordinates.getCoords(radialIndex);
      break;
    case Exterior:
      std::tie(x, y) = _pipeCoordinates.getCoords(_baseConfig.pipeSteps);
      double xSquare, ySquare;
      std::tie(xSquare, ySquare) = _exteriorCoordinates.getCoords(radialIndex + _baseConfig.pipeSteps);
      std::tie(x, y)             = std::make_pair((1 - _pipeToSquareBlend) * x + _pipeToSquareBlend * xSquare,
                                      (1 - _pipeToSquareBlend) * y + _pipeToSquareBlend * ySquare);
      _pipeToSquareBlend += 1. / (_baseConfig.outerSteps - 1);
      break;
  }
  return swapCoordsToQuadrant(quadrant, x, y);
}

std::pair<double, double> CoordinateHelper::swapCoordsToQuadrant(size_t quadrant, double x, double y) {
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
