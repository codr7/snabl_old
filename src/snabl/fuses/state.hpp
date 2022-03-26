#ifndef SNABL_FUSES_STATE_HPP
#define SNABL_FUSES_STATE_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  int state(Fun *fun, M &m);  
}

#endif
