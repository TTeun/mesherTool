#ifndef __NOZZLECOORDINATES__H
#define __NOZZLECOORDINATES__H

#include "../BaseConfig.h"

struct NozzleCoordinates {
  NozzleCoordinates(const BaseConfig &baseConfig) : _baseConfig(baseConfig) {}

  std::pair<double, double> getCoords(const double radialIndex) const;

  void yIndexChanged(const long yIndex);

  double            _absX;
  double            _hP;
  double            _dP;
  double            _hA;
  double            _dA;
  const BaseConfig &_baseConfig;
};

#endif // __NOZZLECOORDINATES__H