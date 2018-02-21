#ifndef __CONFIG__H
#define __CONFIG__H

#include <string>
#include <unordered_map>

struct BaseConfig {
  BaseConfig(char const *baseConfigPath = "config.cfg");

  double _innerSquareWidth;
  double _outerSquareWidth;
  double _innerBlockMinWidth;
  double _innerBlockWidth;
  double _nozzleRadius;
  double _pipeRadius;
  double _alpha;
  double _alphaConnection;
  size_t _innerBlockCount;
  size_t _nozzleSteps;
  size_t _pipeSteps;
  size_t _outerSteps;

  template <typename T>
  T stringToT(std::string const &str);

  template <typename T>
  T readFromMap(std::unordered_map<std::string, std::string> &key_value_map, char const *name);

  void readBaseConfig(char const *config_path);
};

#endif  //__CONFIG__H
