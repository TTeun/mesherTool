#ifndef __COORDINATEHELPER__H
#define __COORDINATEHELPER__H

#include "config.h"
#include "coordinateHelpers/ExteriorCoordinates.h"
#include "coordinateHelpers/NozzleCoordinates.h"
#include "coordinateHelpers/PipeCoordinates.h"
#include <cmath>
#include <limits>

class CoordinateHelper {
  public:
    enum Region {
      Nozzle,
      Pipe,
      Exterior
    };

    CoordinateHelper(const Config &config)
      : _config(config), _nozzleCoordinates(config), _pipeCoordinates(config), _exteriorCoordinates(config)
    {
    }

    void setValues(const long yIndex)
    {
      _nozzleCoordinates.setValues(yIndex);
      _pipeCoordinates.setValues(yIndex);
      _exteriorCoordinates.setValues(yIndex);
      _pipeToSquareBlend = 0;
    }

    std::pair<double, double> getCoords(const long radialIndex, Region region, size_t quadrant);

  private:
    std::pair<double, double> swapCoordsToQuadrant(size_t quadrant, double x, double y);

    double _pipeToSquareBlend;

    const Config        _config;
    NozzleCoordinates   _nozzleCoordinates;
    PipeCoordinates     _pipeCoordinates;
    ExteriorCoordinates _exteriorCoordinates;
};

#endif