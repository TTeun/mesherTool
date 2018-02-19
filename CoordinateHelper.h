#ifndef __COORDINATEHELPER__H
#define __COORDINATEHELPER__H

#include <cmath>
#include <limits>
#include "BaseConfig.h"
#include "coordinateHelpers/ExteriorCoordinates.h"
#include "coordinateHelpers/NozzleCoordinates.h"
#include "coordinateHelpers/PipeCoordinates.h"

class CoordinateHelper {
 public:
  enum Region { Nozzle, Pipe, Exterior };

  CoordinateHelper(const BaseConfig &baseConfig)
      : _baseConfig(baseConfig),
        _nozzleCoordinates(baseConfig),
        _pipeCoordinates(baseConfig),
        _exteriorCoordinates(baseConfig) {}

  void yIndexChanged(const long yIndex) {
    _nozzleCoordinates.yIndexChanged(yIndex);
    _pipeCoordinates.yIndexChanged(yIndex);
    _exteriorCoordinates.yIndexChanged(yIndex);
    _pipeToSquareBlend = 0;
  }

  std::pair<double, double> getCoords(const long radialIndex, Region region, size_t quadrant);

 private:
  std::pair<double, double> swapCoordsToQuadrant(size_t quadrant, double x, double y);

  double _pipeToSquareBlend;

  const BaseConfig    _baseConfig;
  NozzleCoordinates   _nozzleCoordinates;
  PipeCoordinates     _pipeCoordinates;
  ExteriorCoordinates _exteriorCoordinates;
};

#endif