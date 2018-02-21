#include "BaseConfig.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include "io.h"

using namespace std;

BaseConfig::BaseConfig(char const *baseConfigPath) {
  readBaseConfig(baseConfigPath);
  _innerBlockCount = ceil(_innerSquareWidth / _innerBlockMinWidth);
  if (_innerBlockCount % 2) {
    ++_innerBlockCount;
  }
  _innerBlockWidth  = _innerSquareWidth / _innerBlockCount;
  const double phi  = M_PI / (2. * static_cast<double>(_innerBlockCount));
  _nozzleSteps      = std::ceil((std::log2(2. * _nozzleRadius / _innerSquareWidth) / std::log2(1 + phi)));
  _pipeSteps        = std::ceil((std::log2(_pipeRadius / _nozzleRadius) / std::log2(1 + phi)));
  _outerSteps       = 4;
  _outerSquareWidth = _pipeRadius * 1.25;
  _alpha            = 0.23;  // should not be greater than 1 - sin(pi / 4) = 0.29289321881...
  _alphaConnection  = 0.;
}

template <typename T>
T BaseConfig::stringToT(string const &str) {
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
T BaseConfig::readFromMap(unordered_map<string, string> &keyValueMap, char const *name) {
  string stringName(name);
  if (keyValueMap.find(stringName) == keyValueMap.end()) {
    stringName.append(" could not be found in base config file!");
    std::cout << stringName << '\n';
  }
  T result = stringToT<T>(keyValueMap[name]);
  cout << std::setfill('.') << std::setw(30) << left << name << "\t" << result << '\n';
  return result;
}

void BaseConfig::readBaseConfig(char const *baseConfigPath) {
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
  _innerSquareWidth   = readFromMap<double>(keyValueMap, "innerSquareWidth");
  _innerBlockMinWidth = readFromMap<double>(keyValueMap, "innerBlockMinWidth");
  _nozzleRadius       = readFromMap<double>(keyValueMap, "nozzleRadius");
  _pipeRadius         = readFromMap<double>(keyValueMap, "pipeRadius");
  _alpha              = readFromMap<double>(keyValueMap, "alpha");
  _alphaConnection    = readFromMap<double>(keyValueMap, "alphaConnection");
  cout << "BaseConfig file read succesfully\n\n";
}
