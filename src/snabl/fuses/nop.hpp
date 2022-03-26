#ifndef SNABL_FUSES_NOP_HPP
#define SNABL_FUSES_NOP_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  int nop(Fun *fun, M &m);  
}

#endif
