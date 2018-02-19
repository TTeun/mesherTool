#ifndef __PIPECOORDINATES__H
#define __PIPECOORDINATES__H

#include "../BaseConfig.h"

struct PipeCoordinates {
  PipeCoordinates(const BaseConfig &baseConfig);

  std::pair<double, double> getCoords(const long radialIndex) const;

  void yIndexChanged(const long yIndex);

  double _c;
  double _s;
  double _phi;

  const double      _innerRadius;
  const double      _outerRadius;
  const size_t      _numberOfSteps;
  const double      _dr;
  const BaseConfig &_baseConfig;
};

#endif  // __PIPECOORDINATES__H