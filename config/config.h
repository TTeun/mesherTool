#ifndef __CONFIG__H
#define __CONFIG__H

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "../exc/exc.h"
#include <cstddef>

namespace Mesher {

  struct Config {
    Config(char const *config_path);

    double inner_square_side;
    double inner_block_min_width;
    double inner_block_width;
    double half_square_size;

    size_t inner_block_count;
    size_t half_inner_block_count;
    size_t outer_steps;
    double circle_radius;
    double alpha;
    double alpha_connection;
    std::ofstream node;
    std::ofstream poly;

    template<typename T>
    T string_to_T(std::string const &str);

    template <typename T>
    T readFromMap(std::unordered_map<std::string, std::string> &key_value_map, char const * name);

    void readConfig(char const *config_path);
    void printHeaders();
  };
} // Mesher


#endif //__CONFIG__H
