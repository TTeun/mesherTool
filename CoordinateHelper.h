#ifndef __COORDINATEHELPER__H
#define __COORDINATEHELPER__H

#include "config.h"
#include "coordinateHelpers/Region1.h"
#include "coordinateHelpers/Region2.h"
#include "coordinateHelpers/Region3.h"
#include <cmath>
#include <limits>

class CoordinateHelper
{
public:
  enum Region
  {
    R1,
    R2,
    R3
  };

  CoordinateHelper(const Config &config)
      : _config(config), _region1(config), _region2(config), _region3(config)
  {
  }

  void setValues(const long yIndex)
  {
    _region1.setValues(yIndex);
    _region2.setValues(yIndex);
    _region3.setValues(yIndex);
    _blend = 0;
  }

  std::pair<double, double> getCoords(const long radialIndex, Region region, size_t quadrant);

private:
  void swapCoordsToQuadrant(size_t quadrant, std::pair<double, double> &coords);

  double _x;
  double _y;
  double _blend;

  const Config _config;
  Region1      _region1;
  Region2      _region2;
  Region3      _region3;
};

#endif