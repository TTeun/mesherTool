#include "BaseConfig.h"
#include "io.h"
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

BaseConfig::BaseConfig(char const *baseConfigPath)
{
  readBaseConfig(baseConfigPath);
  innerBlockCount = ceil(innerSquareSide / innerBlockMinWidth);
  if (innerBlockCount % 2) {
    ++innerBlockCount;
  }
  innerBlockWidth     = innerSquareSide / innerBlockCount;
  halfInnerBlockCount = innerBlockCount / 2;
  halfSquareSize      = innerSquareSide / 2;
  nozzleSteps         = (nozzleRadius - halfSquareSize) / innerBlockWidth + 1;
  const double phi    = M_PI / (4. * static_cast<double>(halfInnerBlockCount));
  pipeSteps  = std::ceil((std::log2(pipeRadius / static_cast<double>(nozzleRadius)) / std::log2(1 + phi)));
  outerSteps = 4;
  totalSteps = pipeSteps + nozzleSteps + outerSteps + 1;
  outerSquareSide = pipeRadius * 1.25;
}

template <typename T>
T BaseConfig::string_to_T(string const &str)
{
  T            result;
  stringstream sstream(str);
  if (not(sstream >> result)) {
    string error("Cannot not convert ");
    error.append(str);
    error.append(" to appropriate value\n");
    std::cout << error << '\n';
  }
  return result;
}

template <typename T>
T BaseConfig::readFromMap(unordered_map<string, string> &keyValueMap, char const *name)
{
  string stringName(name);
  if (keyValueMap.find(stringName) == keyValueMap.end()) {
    stringName.append(" could not be found in base config file!");
    std::cout << stringName << '\n';
  }
  T result = string_to_T<T>(keyValueMap[name]);
  cout << std::setfill('.') << std::setw(30) << left << name << "\t" << result << '\n';
  return result;
}

void BaseConfig::readBaseConfig(char const *baseConfigPath)
{
  ifstream                      conf = IO::open_ifstream(baseConfigPath, std::ofstream::in);
  string                        line;
  unordered_map<string, string> keyValueMap;
  string                        key, value;
  while (getline(conf, line)) {
    stringstream lineStream(line);
    lineStream >> key >> value;
    if (keyValueMap.find(key) != keyValueMap.end()) {
      string error("Double definition of ");
      error.append(key);
      error.append(" in BaseConfiguration file");
      std::cout << error << '\n';
    }
    keyValueMap.insert(make_pair(key, value));
  }
  innerSquareSide    = readFromMap<double>(keyValueMap, "innerSquareSide");
  innerBlockMinWidth = readFromMap<double>(keyValueMap, "innerBlockMinWidth");
  nozzleRadius       = readFromMap<double>(keyValueMap, "nozzleRadius");
  pipeRadius         = readFromMap<double>(keyValueMap, "pipeRadius");
  alpha              = readFromMap<double>(keyValueMap, "alpha");
  alphaConnection    = readFromMap<double>(keyValueMap, "alphaConnection");
  cout << "BaseConfig file read succesfully\n\n";
}
