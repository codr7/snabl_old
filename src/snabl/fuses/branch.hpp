#ifndef SNABL_FUSES_BRANCH_HPP
#define SNABL_FUSES_BRANCH_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  int branch(Fun *fun, M &m);  
}

#endif
