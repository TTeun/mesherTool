#ifndef __REGION2__H
#define __REGION2__H

#include "../config.h"

struct Region2
{
  Region2(double innerRadius, double outerRadius, size_t numberOfSteps, const Config &config);

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

#endif // __REGION2_H