#ifndef SNABL_FUSES_UTIL_HPP
#define SNABL_FUSES_UTIL_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  using namespace snabl;
  
  pair<PC, vector<PC>> drill_pc(PC pc, M &m);
}

#endif
