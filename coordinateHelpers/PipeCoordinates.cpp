#include "PipeCoordinates.h"
#include <cmath>
#include <iostream>

template <typename T>
T sq(const T &val)
{
  return val * val;
}

PipeCoordinates::PipeCoordinates(const BaseConfig &baseConfig)
  : _innerRadius(baseConfig.nozzleRadius),
    _outerRadius(baseConfig.pipeRadius),
    _numberOfSteps(baseConfig.pipeSteps),
    _dr((_outerRadius - _innerRadius) / static_cast<double>(_numberOfSteps)),
    _baseConfig(baseConfig)
{
  _c = std::pow(_outerRadius / _innerRadius, 1. / _numberOfSteps) - 1;
  std::cout << _c << '\n';
}

std::pair<double, double> PipeCoordinates::getCoords(const long radialIndex) const
{
  // double r = _innerRadius + (radialIndex + 1.) * _dr;
  double r = _innerRadius * std::pow(1 + _c, radialIndex + 1);
  return std::make_pair(r * cos(_phi), r * sin(_phi));
}

void PipeCoordinates::yIndexChanged(const long yIndex)
{
  _s   = yIndex / static_cast<double>(_baseConfig.halfInnerBlockCount);
  _phi = M_PI * _s / 4.;
}
