#include "ExteriorCoordinates.h"
#include <cmath>

template <typename T>
T sq(const T &val)
{
  return val * val;
}

std::pair<double, double> ExteriorCoordinates::getCoords(const long radialIndex) const
{
  const double x = _pipeRadius + radialIndex * _dr;
  return std::make_pair(x, std::tan(_phi) * x);
}

void ExteriorCoordinates::setValues(const long yIndex)
{
  double s = yIndex / static_cast<double>(_config.halfInnerBlockCount);
  _phi     = M_PI * s / 4.;
}
