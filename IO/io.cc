#include "io.h"
#include "../exc/exc.h"

namespace IO {
  ofstream open_ofstream(char const * path, ios_base::openmode mode){
    ofstream str(path, mode);
    if (not str.is_open()){
      string error("Could not open file ");
      error.append(path);
      throw Exc(error, Exc::EXC_TYPE::ERROR);
    }
    return str;
  }

  ifstream open_ifstream(char const * path, ios_base::openmode mode){
    ifstream str(path, mode);
    if (not str.is_open()){
      string error("Could not open file ");
      error.append(path);
      throw Exc(error, Exc::EXC_TYPE::ERROR);
    }
    return str;
  }
} // IO
