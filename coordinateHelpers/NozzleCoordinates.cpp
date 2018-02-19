#include "NozzleCoordinates.h"
#include <cmath>

template <typename T>
T sq(const T &val)
{
  return val * val;
}

std::pair<double, double> NozzleCoordinates::getCoords(const double radialIndex) const
{
  double       blend  = radialIndex / (_baseConfig.nozzleSteps - 1.);
  const double absRad = _baseConfig.alphaConnection * (1. - blend);
  const double rP =
    1. + (radialIndex + 1) * (_baseConfig.nozzleRadius / _dP - 1.) / (_baseConfig.nozzleSteps);
  const double xP = rP * (1. - _baseConfig.alpha * sq(_absX) * absRad) * _baseConfig.halfSquareSize;
  const double yP = rP * (1. - absRad * _baseConfig.alpha) * _hP;
  const double rA =
    1. + (radialIndex + 1) * (_baseConfig.nozzleRadius / _dA - 1.) / (_baseConfig.nozzleSteps);
  const double xA = rA * (1. - _baseConfig.alpha * sq(_absX) * absRad) * _baseConfig.halfSquareSize;
  const double yA = rA * (1. - absRad * _baseConfig.alpha) * _hA;
  return std::make_pair((1. - blend) * xP + blend * xA, (1. - blend) * yP + blend * yA);
}

void NozzleCoordinates::yIndexChanged(const long yIndex)
{
  const double s   = yIndex / static_cast<double>(_baseConfig.halfInnerBlockCount);
  const double phi = M_PI * s / 4.;
  _absX            = std::abs(s);
  _hP              = s * _baseConfig.halfSquareSize;
  _dP              = sqrt(_hP * _hP + _baseConfig.halfSquareSize * _baseConfig.halfSquareSize);
  _hA              = tan(phi) * _baseConfig.halfSquareSize;
  _dA              = sqrt(_hA * _hA + _baseConfig.halfSquareSize * _baseConfig.halfSquareSize);
}
