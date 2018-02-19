#ifndef __EXTERIORCOORDINATES__H
#define __EXTERIORCOORDINATES__H

#include <iostream>
#include "../BaseConfig.h"

struct ExteriorCoordinates {
  ExteriorCoordinates(const BaseConfig &baseConfig)
      : _outerSquareSize(baseConfig.outerSquareSide),
        _pipeRadius(baseConfig.pipeRadius),
        _dr((baseConfig.outerSquareSide - _pipeRadius) / static_cast<double>(baseConfig.outerSteps)),
        _baseConfig(baseConfig) {}

  std::pair<double, double> getCoords(const long radialIndex) const;

  void yIndexChanged(const long yIndex);

  double _phi;

  double            _outerSquareSize;
  const double      _pipeRadius;
  const double      _dr;
  const BaseConfig &_baseConfig;
};

#endif  // __EXTERIORCOORDINATES__H