#ifndef __CONFIG__H
#define __CONFIG__H

#include <string>
#include <unordered_map>

struct BaseConfig {
  BaseConfig(char const *baseConfigPath = "config.cfg");

  double innerSquareSide;
  double outerSquareSide;
  double innerBlockMinWidth;
  double innerBlockWidth;
  double halfSquareSize;

  size_t innerBlockCount;
  size_t halfInnerBlockCount;
  size_t nozzleSteps;
  double nozzleRadius;
  size_t pipeSteps;
  double pipeRadius;

  size_t outerSteps;

  size_t totalSteps;

  double alpha;
  double alphaConnection;

  template <typename T>
  T string_to_T(std::string const &str);

  template <typename T>
  T readFromMap(std::unordered_map<std::string, std::string> &key_value_map, char const *name);

  void readBaseConfig(char const *config_path);
};

#endif  //__CONFIG__H
