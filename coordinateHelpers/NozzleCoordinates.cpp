#include "NozzleCoordinates.h"

template <typename T>
T sq(const T &val) {
  return val * val;
}

std::pair<double, double> NozzleCoordinates::getCoords(const double radialIndex) const {
  const double r = _baseConfig.halfSquareSize * std::pow(1. + _c, radialIndex);
  return std::make_pair(r * cos(_phi), r * sin(_phi));
}

void NozzleCoordinates::yIndexChanged(const long yIndex) {
  const double s = yIndex / static_cast<double>(_baseConfig.halfInnerBlockCount);
  _phi           = M_PI * s / 4.;
}
