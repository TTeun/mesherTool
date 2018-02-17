#include "Region3.h"
#include <cmath>

template <typename T>
T sq(const T &val)
{
  return val * val;
}

std::pair<double, double> Region3::getCoords(const long radialIndex) const
{
  const double x = _pipeRadius + radialIndex * _dr;
  return std::make_pair(x, std::tan(_phi) * x);
}

void Region3::setValues(const long yIndex)
{
  double s = yIndex / static_cast<double>(_config.halfInnerBlockCount);
  _phi     = M_PI * s / 4.;
}
