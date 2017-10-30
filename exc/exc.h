#ifndef __EXC__H
#define __EXC__H

#include <cexcept>
#include <string>

using namespace std;

class Exc : public exception {
public:
  Exc(const char *desc);

  string what();
  enum class EXC_TYPE {
    INFO,
    WARNING,
    ERROR
  };

};

#endif // __EXC__H
