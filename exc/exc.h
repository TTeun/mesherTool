#ifndef __EXC__H
#define __EXC__H

#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

class Exc : public exception {
public:
  enum class EXC_TYPE {
    INFO,
    WARNING,
    ERROR
  };

  Exc() = delete;
  Exc(const char *errMessage, EXC_TYPE type);
  Exc(string const &errMessage, EXC_TYPE type);
  virtual ~Exc() throw();

  friend ostream& operator<<(ostream &ostr, Exc const &e);
protected:
  char const * m_errMessage = 0;
  EXC_TYPE m_type;
};

#endif // __EXC__H
