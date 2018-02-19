#ifndef __EXTERIORCOORDINATES__H
#define __EXTERIORCOORDINATES__H

#include "../config.h"

struct ExteriorCoordinates {
  ExteriorCoordinates(const Config &config)
    : _pipeRadius(config.pipeRadius),
      _dr((config.outerSquareSide - _pipeRadius) / static_cast<double>(config.outerSteps)),
      _config(config)
  {
  }

  std::pair<double, double> getCoords(const long radialIndex) const;

  void setValues(const long yIndex);

  double _phi;

  const double  _pipeRadius;
  const double  _dr;
  const Config &_config;
};

#endif // __EXTERIORCOORDINATES__H