#include "CoordinateHelper.h"
#include <cmath>
#include <iostream>

std::pair<double, double> CircleAroundSquareCoordinates::getXY(const double radialIndex)
{
  double blend = (radialIndex - 1.) / (_config.nozzleSteps - 2.);

  const double absRad = _config.alphaConnection * (1. - blend);
  const double rP     = 1. + radialIndex * (_config.nozzleRadius / _dP - 1.) / (_config.nozzleSteps - 1.);
  const double xP     = rP * (1. - _config.alpha * sq(_absX) * absRad) * _config.halfSquareSize;
  const double yP     = rP * (1. - absRad * _config.alpha) * _hP;
  const double rA     = 1. + radialIndex * (_config.nozzleRadius / _dA - 1.) / (_config.nozzleSteps - 1.);
  const double xA     = rA * (1. - _config.alpha * sq(_absX) * absRad) * _config.halfSquareSize;
  const double yA     = rA * (1. - absRad * _config.alpha) * _hA;

  return std::make_pair((1. - blend) * xP + blend * xA, (1. - blend) * yP + blend * yA);
}

void CircleAroundSquareCoordinates::setValues(const long xIndex)
{
  _s    = xIndex / static_cast<double>(_config.halfInnerBlockCount);
  _absX = std::abs(_s);
  _hP   = _s * _config.halfSquareSize;
  _dP   = sqrt(_hP * _hP + _config.halfSquareSize * _config.halfSquareSize);
  _phi  = M_PI * _s / 4.;
  _hA   = tan(_phi) * _config.halfSquareSize;
  _dA   = sqrt(_hA * _hA + _config.halfSquareSize * _config.halfSquareSize);
}

CircleCoordinates::CircleCoordinates(double        innerRadius,
                                     double        outerRadius,
                                     size_t        numberOfSteps,
                                     const Config &config)
    : _innerRadius(innerRadius), _outerRadius(outerRadius), _numberOfSteps(numberOfSteps), _config(config)
{
  const double dr = _outerRadius - _innerRadius;
  const double d  = _numberOfSteps * _innerRadius + dr * (_numberOfSteps + 1.) / 2.;
  _c              = dr / d;
  _r              = _innerRadius;
}

std::pair<double, double> CircleCoordinates::getXY(const long xIndex, const long radialIndex)
{
  _r += (1.) / static_cast<double>(_numberOfSteps) * (_outerRadius - _innerRadius);
  // const double dr = _outerRadius - _innerRadius;
  // _r =  _innerRadius + _c * (radialIndex * (radialIndex - 1.)) * dr /
  // static_cast<double>(dr); std::cout << _r << '\n';
  return std::make_pair(_r * cos(_phi), _r * sin(_phi));
}

void CircleCoordinates::setValues(const long xIndex)
{
  _s   = xIndex / static_cast<double>(_config.halfInnerBlockCount);
  _phi = M_PI * _s / 4.;
  _r   = _innerRadius;
}

std::pair<double, double> SquareAroundCircleCoordinates::getXY(const long xIndex, const long radialIndex)
{
  const double x = _pipeRadius + radialIndex * _dr;
  return std::make_pair(x, std::tan(_phi) * x);
}

void SquareAroundCircleCoordinates::setValues(const long xIndex)
{
  double s = xIndex / static_cast<double>(_config.halfInnerBlockCount);
  _phi     = M_PI * s / 4.;
}
