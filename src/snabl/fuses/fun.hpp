#ifndef SNABL_FUSES_FUN_HPP
#define SNABL_FUSES_FUN_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  void fun(Fun *fun, M &m);  
  int fun_entry(Fun *fun, M &m);
  int fun_tail(Fun *fun, M &m);
}

#endif
