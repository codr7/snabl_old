#ifndef SNABL_FUSES_TAIL_CALL_HPP
#define SNABL_FUSES_TAIL_CALL_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  int tail_call(Fun *fun, M &m);  
}

#endif
