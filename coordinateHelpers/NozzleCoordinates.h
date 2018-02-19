#ifndef __NOZZLECOORDINATES__H
#define __NOZZLECOORDINATES__H

#include "../config.h"

struct NozzleCoordinates {
  NozzleCoordinates(const Config &config) : _config(config) {}

  std::pair<double, double> getCoords(const double radialIndex) const;

  void setValues(const long yIndex);

  double        _absX;
  double        _hP;
  double        _dP;
  double        _hA;
  double        _dA;
  const Config &_config;
};

#endif // __NOZZLECOORDINATES__H