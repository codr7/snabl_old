#ifndef SNABL_FUSES_FUN_HPP
#define SNABL_FUSES_FUN_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  void fun(Fun *fun, PC start_pc, M &m);  
  PC fun_entry(Fun *fun, PC start_pc, M &m);
}

#endif
