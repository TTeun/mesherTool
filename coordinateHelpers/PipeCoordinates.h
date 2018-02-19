#ifndef __PIPECOORDINATES__H
#define __PIPECOORDINATES__H

#include "../config.h"

struct PipeCoordinates {
  PipeCoordinates(const Config &config);

  std::pair<double, double> getCoords(const long radialIndex) const;

  void setValues(const long yIndex);

  double _c;
  double _s;
  double _phi;

  const double  _innerRadius;
  const double  _outerRadius;
  const size_t  _numberOfSteps;
  const double  _dr;
  const Config &_config;
};

#endif // __PIPECOORDINATES__H