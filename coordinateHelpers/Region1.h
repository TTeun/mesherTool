#ifndef __REGION1__H
#define __REGION1__H

#include "../config.h"

struct Region1
{
  Region1(const Config &config) : _config(config) {}

  std::pair<double, double> getCoords(const double radialIndex) const;

  void setValues(const long yIndex);

  double        _s;
  double        _absX;
  double        _hP;
  double        _dP;
  double        _phi;
  double        _hA;
  double        _dA;
  const Config &_config;
};

#endif // __REGION1_H