#include "config.h"
#include "io.h"
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

Config::Config(char const *configPath)
{
  readConfig(configPath);
  innerBlockCount = ceil(innerSquareSide / innerBlockMinWidth);
  if (innerBlockCount % 2)
  {
    ++innerBlockCount;
  }

  innerBlockWidth     = innerSquareSide / innerBlockCount;
  halfInnerBlockCount = innerBlockCount / 2;
  halfSquareSize      = innerSquareSide / 2;
  nozzleSteps         = (nozzleRadius - halfSquareSize) / innerBlockWidth + 1;

  const double dr  = pipeRadius - nozzleRadius;
  const double phi = M_PI / (2. * static_cast<double>(halfInnerBlockCount));
  const double num = dr / phi - dr / 2.;
  const double den = nozzleRadius + dr / 2.;
  const double k   = num / den;

  pipeSteps       = std::ceil(k);
  pipeSteps       = 12;
  outerSteps      = 7;
  totalSteps      = pipeSteps + nozzleSteps + outerSteps + 1;
  outerSquareSide = pipeRadius * 1.25;
}

template <typename T>
T Config::string_to_T(string const &str)
{
  T            result;
  stringstream sstream(str);
  if (not(sstream >> result))
  {
    string error("Cannot not convert ");
    error.append(str);
    error.append(" to appropriate value\n");
    std::cout << error << '\n';
  }
  return result;
}

template <typename T>
T Config::readFromMap(unordered_map<string, string> &keyValueMap, char const *name)
{
  string stringName(name);
  if (keyValueMap.find(stringName) == keyValueMap.end())
  {
    stringName.append(" could not be found in config file!");
    std::cout << stringName << '\n';
  }
  T result = string_to_T<T>(keyValueMap[name]);
  cout << std::setfill('.') << std::setw(30) << left << name << "\t" << result << '\n';
  return result;
}

void Config::readConfig(char const *configPath)
{
  ifstream                      conf = IO::open_ifstream(configPath, std::ofstream::in);
  string                        line;
  unordered_map<string, string> keyValueMap;
  string                        key, value;
  while (getline(conf, line))
  {
    stringstream lineStream(line);
    lineStream >> key >> value;
    if (keyValueMap.find(key) != keyValueMap.end())
    {
      string error("Double definition of ");
      error.append(key);
      error.append(" in configuration file");
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
  cout << "Config file read succesfully\n\n";
}
