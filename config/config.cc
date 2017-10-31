#include "config.h"
#include "../IO/io.h"
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

namespace Mesher {
  Config::Config(){
    readConfig();
    inner_block_count     = ceil(inner_square_side / inner_block_min_width);
    if (inner_block_count % 2)
      ++inner_block_count;

    inner_block_width       = inner_square_side / inner_block_count;
    half_inner_block_count  = inner_block_count / 2;
    half_square_size        = inner_square_side / 2;
    outer_steps   = (circle_radius - half_square_size) / inner_block_width + 1;

    node = IO::open_ofstream("data/dat.node", std::ofstream::out | std::ofstream::trunc);
    poly = IO::open_ofstream("data/dat.poly", std::ofstream::out | std::ofstream::trunc);
  }

  template<typename T>
  T Config::string_to_T(string const &str){
    T result;
    stringstream sstream(str);
    if (not (sstream >> result))
    {
      string error("Cannot not convert ");
      error.append(str);
      error.append(" to appropriate value\n");
      throw Exc(move(error), Exc::EXC_TYPE::ERROR);
    }
    return result;
  }

  template <typename T>
  T Config::readFromMap(unordered_map<string, string> &key_value_map, char const * name){
    string str_name(name);
    if (key_value_map.find(str_name) == key_value_map.end())
    {
      str_name.append(" could not be found in config file!");
      throw Exc(move(str_name), Exc::EXC_TYPE::ERROR);
    }
    T result = string_to_T<T>( key_value_map[name] );
    cout << std::setfill ('.') << std::setw(30) << left << name << "\t" << result << '\n';
    return result;
  }

  void Config::readConfig(){
    ifstream conf = IO::open_ifstream("config.cfg", std::ofstream::in);
    string line;
    unordered_map<string, string> key_value_map;
    string key, value;
    while (getline(conf, line))
    {
      stringstream lineStream(line);
      lineStream >> key >> value;
      if (key_value_map.find(key) != key_value_map.end())
      {
        string error("Double definition of ");
        error.append(key);
        error.append(" in configuration file");
        throw Exc(move(error), Exc::EXC_TYPE::ERROR);
      }

      key_value_map.insert(make_pair(key, value));
    }

    inner_square_side     = readFromMap<double>(key_value_map, "inner_square_side");
    inner_block_min_width = readFromMap<double>(key_value_map, "inner_block_min_width");
    circle_radius         = readFromMap<double>(key_value_map, "circle_radius");
    alpha                 = readFromMap<double>(key_value_map, "alpha");
    alpha_connection      = readFromMap<double>(key_value_map, "alpha_connection");
    cout << "Config file read succesfully\n\n";
  }

  void Config::printHeaders(){
    node << (inner_block_count + 1) * (inner_block_count + 1) + 4 * (outer_steps - 1) * (inner_block_count + 1) << '\n';
    poly << inner_block_count * inner_block_count + 4 * (outer_steps - 1) * inner_block_count << '\n';
  }


} // Mesher
