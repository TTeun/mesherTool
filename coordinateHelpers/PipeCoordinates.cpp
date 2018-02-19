#include "PipeCoordinates.h"
#include <cmath>

template <typename T>
T sq(const T &val)
{
  return val * val;
}

PipeCoordinates::PipeCoordinates(const Config &config)
  : _innerRadius(config.nozzleRadius),
    _outerRadius(config.pipeRadius),
    _numberOfSteps(config.pipeSteps),
    _dr((_outerRadius - _innerRadius) / static_cast<double>(_numberOfSteps)),
    _config(config)
{
  // const double dr = _outerRadius - _innerRadius;
  // const double d  = _numberOfSteps * _innerRadius + dr * (_numberOfSteps + 1.) / 2.;
  // _c              = dr / d;
}

std::pair<double, double> PipeCoordinates::getCoords(const long radialIndex) const
{
  double r = _innerRadius + (radialIndex + 1.) * _dr;
  // const double dr = _outerRadius - _innerRadius;
  // _r =  _innerRadius + _c * (radialIndex * (radialIndex - 1.)) * dr /
  // static_cast<double>(dr); std::cout << _r << '\n';
  return std::make_pair(r * cos(_phi), r * sin(_phi));
}

void PipeCoordinates::setValues(const long yIndex)
{
  _s   = yIndex / static_cast<double>(_config.halfInnerBlockCount);
  _phi = M_PI * _s / 4.;
}
