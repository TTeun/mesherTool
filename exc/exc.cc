#include "exc.h"

using namespace std;

Exc::Exc(string && errMessage, EXC_TYPE type)
 : m_errMessage(errMessage), m_type(type)
{
}

Exc::~Exc(){}

ostream& operator<<(ostream &ostr, Exc const &e){
  ostr << "======== " << (
                         e.m_type == Exc::EXC_TYPE::ERROR   ? "Error"   :
                         e.m_type == Exc::EXC_TYPE::WARNING ? "Warning" :
                         e.m_type == Exc::EXC_TYPE::INFO    ? "Info"    :
                         "Severity not set, should be impossible"
                       ) << " ========\n";

  ostr << '\t' << e.m_errMessage << '\n';
  return ostr;
}
