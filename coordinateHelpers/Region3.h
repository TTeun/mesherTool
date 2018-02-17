#ifndef __REGION3__H
#define __REGION3__H

#include "../config.h"

struct Region3
{
  Region3(const Config &config)
      : _pipeRadius(config.pipeRadius),
        _squareSize(config.outerSquareSide),
        _numberOfSteps(config.outerSteps),
        _dr((_squareSize - _pipeRadius) / static_cast<double>(_numberOfSteps)),
        _config(config)
  {
  }

  std::pair<double, double> getCoords(const long radialIndex) const;

  void setValues(const long yIndex);

  double _phi;

  const double  _pipeRadius;
  const double  _squareSize;
  const size_t  _numberOfSteps;
  const double  _dr;
  const Config &_config;
};

#endif // __REGION3_H