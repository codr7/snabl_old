#ifndef SNABL_FUSES_GOTO_RET_HPP
#define SNABL_FUSES_GOTO_RET_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  int goto_ret(Fun *fun, PC start_pc, M &m);  
}

#endif
