#include "Region1.h"
#include <cmath>

template <typename T>
T sq(const T &val)
{
  return val * val;
}

std::pair<double, double> Region1::getCoords(const double radialIndex) const
{
  double blend = radialIndex / (_config.nozzleSteps - 1.);

  const double absRad = _config.alphaConnection * (1. - blend);
  const double rP     = 1. + (radialIndex + 1) * (_config.nozzleRadius / _dP - 1.) / (_config.nozzleSteps);
  const double xP     = rP * (1. - _config.alpha * sq(_absX) * absRad) * _config.halfSquareSize;
  const double yP     = rP * (1. - absRad * _config.alpha) * _hP;
  const double rA     = 1. + (radialIndex + 1) * (_config.nozzleRadius / _dA - 1.) / (_config.nozzleSteps);
  const double xA     = rA * (1. - _config.alpha * sq(_absX) * absRad) * _config.halfSquareSize;
  const double yA     = rA * (1. - absRad * _config.alpha) * _hA;

  return std::make_pair((1. - blend) * xP + blend * xA, (1. - blend) * yP + blend * yA);
}

void Region1::setValues(const long yIndex)
{
  _s    = yIndex / static_cast<double>(_config.halfInnerBlockCount);
  _absX = std::abs(_s);
  _hP   = _s * _config.halfSquareSize;
  _dP   = sqrt(_hP * _hP + _config.halfSquareSize * _config.halfSquareSize);
  _phi  = M_PI * _s / 4.;
  _hA   = tan(_phi) * _config.halfSquareSize;
  _dA   = sqrt(_hA * _hA + _config.halfSquareSize * _config.halfSquareSize);
}
