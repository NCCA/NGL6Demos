#include "fuzzy.h"
using namespace std;
ostream& operator<<(ostream& output, const fuzzy& s)
  {
    return output<<s.m_fNum<<endl;
  }
