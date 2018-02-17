#ifndef __COORDINATEHELPER__H
#define __COORDINATEHELPER__H

#include "config.h"
#include <cmath>

template <typename T>
T sq(const T &a)
{
  return a * a;
}

struct CircleAroundSquareCoordinates
{
  CircleAroundSquareCoordinates(const Config &config) : _config(config) {}

  std::pair<double, double> getXY(const double radialIndex);

  void setValues(const long xIndex);

  double        _s;
  double        _absX;
  double        _hP;
  double        _dP;
  double        _phi;
  double        _hA;
  double        _dA;
  const Config &_config;
};

struct CircleCoordinates
{
  CircleCoordinates(double innerRadius, double outerRadius, size_t numberOfSteps, const Config &config);

  std::pair<double, double> getXY(const long xIndex, const long radialIndex);

  void setValues(const long xIndex);

  double _r;
  double _c;
  double _s;
  double _phi;

  const double  _innerRadius;
  const double  _outerRadius;
  const size_t  _numberOfSteps;
  const Config &_config;
};

struct SquareAroundCircleCoordinates
{
  SquareAroundCircleCoordinates(const double  circleRadius,
                                const double  squareSize,
                                const size_t  numberOfSteps,
                                const Config &config)
      : _pipeRadius(circleRadius),
        _squareSize(squareSize),
        _numberOfSteps(numberOfSteps),
        _dr((_squareSize - _pipeRadius) / static_cast<double>(_numberOfSteps)),
        _config(config)
  {
  }

  std::pair<double, double> getXY(const long xIndex, const long radialIndex);

  void setValues(const long xIndex);

  double _phi;

  const double  _pipeRadius;
  const double  _squareSize;
  const size_t  _numberOfSteps;
  const double  _dr;
  const Config &_config;
};

class CoordinateHelper
{
public:
  enum Region
  {
    Region1,
    Region2,
    Region3
  };

  CoordinateHelper(const Config &config)
      : _region1(config),
        _region2(config.nozzleRadius, config.pipeRadius, config.pipeSteps, config),
        _region3(config.pipeRadius, config.outerSquareSide, config.outerSteps, config)
  {
  }

  void setValues(const long xIndex)
  {
    _region1.setValues(xIndex);
    _region2.setValues(xIndex);
    _region3.setValues(xIndex);
  }

  std::pair<double, double> getCoords(const long radialIndex, Region region, size_t quadrant)
  {
    switch (region)
    {
    case Region1:
      std::tie(_x, _y) = _region1.getXY(radialIndex);
      for (size_t q = 0; q != quadrant; ++q)
      {
        std::tie(_x, _y) = std::make_pair(-_y, _x);
      }
      return std::make_pair(_x, _y);
      break;
    case Region2:
      std::tie(_x, _y) = _region2.getXY(0, radialIndex);
      for (size_t q = 0; q != quadrant; ++q)
      {
        std::tie(_x, _y) = std::make_pair(-_y, _x);
      }
      return std::make_pair(_x, _y);
      break;
    }
  }

private:
  double _x;
  double _y;

  CircleAroundSquareCoordinates _region1;
  CircleCoordinates             _region2;
  SquareAroundCircleCoordinates _region3;
};

#endif