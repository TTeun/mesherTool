#include "CoordinateHelper.h"
#include <cmath>
#include <iostream>

std::pair<double, double> CoordinateHelper::getCoords(const long radialIndex, Region region, size_t quadrant)
{
  double x;
  double y;
  switch (region)
  {
  case R1:
    std::tie(x, y) = _region1.getCoords(radialIndex);
    for (size_t q = 0; q != quadrant; ++q)
    {
      std::tie(x, y) = std::make_pair(-y, x);
    }
    return std::make_pair(x, y);
    break;
  case R2:
    std::tie(x, y) = _region2.getCoords(radialIndex);
    for (size_t q = 0; q != quadrant; ++q)
    {
      std::tie(x, y) = std::make_pair(-y, x);
    }
    return std::make_pair(x, y);
    break;
  case R3:
    std::tie(x, y) = _region2.getCoords(_config.pipeSteps);
    double xS, yS;
    std::tie(xS, yS) = _region3.getCoords(radialIndex + _config.pipeSteps);
    std::tie(x, y)   = std::make_pair((1 - _blend) * x + _blend * xS, (1 - _blend) * y + _blend * yS);
    _blend += 1. / (_config.outerSteps - 1);

    for (size_t q = 0; q != quadrant; ++q)
    {
      std::tie(x, y) = std::make_pair(-y, x);
    }
    return std::make_pair(x, y);
    break;
  }

  return std::pair<double, double>(std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
}

void CoordinateHelper::swapCoordsToQuadrant(size_t quadrant, std::pair<double, double> &coords)
{
  switch (quadrant)
  {
  case 0:
    return;
  case 1:
    std::tie(coords.first, coords.second) = std::make_pair(-coords.second, coords.first);
    return;
  case 2:
    std::tie(coords.first, coords.second) = std::make_pair(-coords.first, -coords.second);
    return;
  case 3:
    std::tie(coords.first, coords.second) = std::make_pair(coords.second, -coords.first);
    return;
  }
}
