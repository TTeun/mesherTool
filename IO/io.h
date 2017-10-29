#ifndef __IO__H
#define __IO__H

#include <fstream>

using namespace std;

namespace IO {
  ofstream open_ofstream(char const * path, ios_base::openmode mode = ios_base::out);
  ifstream open_ifstream(char const * path, ios_base::openmode mode = ios_base::in);
} // IO

#endif // __IO
