#ifndef __CONFIG__H
#define __CONFIG__H

#include <string>
#include <unordered_map>

struct Config {
  Config(char const *config_path = "config.cfg");

  double inner_square_side;
  double inner_block_min_width;
  double inner_block_width;
  double half_square_size;

  size_t inner_block_count;
  size_t half_inner_block_count;
  size_t nozzle_steps;
  double nozzle_radius;
  size_t pipe_steps;
  double pipe_radius;

  size_t outer_steps;

  double alpha;
  double alpha_connection;

  template<typename T>
  T string_to_T(std::string const &str);

  template <typename T>
  T readFromMap(std::unordered_map<std::string, std::string> &key_value_map, char const * name);

  void readConfig(char const *config_path);
};

#endif //__CONFIG__H
