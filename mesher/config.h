#ifndef __CONFIG__H
#define __CONFIG__H

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "../exc/exc.h"
#include <cstddef>

using namespace std;

namespace MeshBuilder {

  struct Config {
    Config();

    double inner_square_side;
    double inner_block_min_width;
    double inner_block_width;
    double half_square_size;

    size_t inner_block_count;
    size_t half_inner_block_count;
    size_t outer_steps;
    double circle_radius;
    float alpha;
    std::ofstream node;
    std::ofstream poly;

    template<typename T>
    T string_to_T(string const &str);

    template <typename T>
    T readFromMap(unordered_map<string, string> &key_value_map, char const * name);

    void readConfig();
    void printHeaders();
  };
} // MeshBuilder


#endif //__CONFIG__H
