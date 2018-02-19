#ifndef __NOZZLECOORDINATES__H
#define __NOZZLECOORDINATES__H

#include <cmath>
#include "../BaseConfig.h"

struct NozzleCoordinates {
  NozzleCoordinates(const BaseConfig &baseConfig) : _baseConfig(baseConfig) {
    _c =
        std::pow(_baseConfig.nozzleRadius / _baseConfig.halfSquareSize, 1. / (_baseConfig.nozzleSteps - 1.)) -
        1.;
  }

  std::pair<double, double> getCoords(const double radialIndex) const;

  void yIndexChanged(const long yIndex);

  double _c;

  double            _phi;
  const BaseConfig &_baseConfig;
};

#endif  // __NOZZLECOORDINATES__H