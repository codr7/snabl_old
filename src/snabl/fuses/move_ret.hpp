#ifndef SNABL_FUSES_MOVE_RET_HPP
#define SNABL_FUSES_MOVE_RET_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  int move_ret(Fun *fun, PC start_pc, M &m);  
}

#endif
