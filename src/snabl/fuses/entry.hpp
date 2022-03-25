#ifndef SNABL_FUSES_ENTRY_HPP
#define SNABL_FUSES_ENTRY_HPP

#include "snabl/types.hpp"

namespace snabl {
  struct M;
}

namespace snabl::fuses {
  int entry(Fun *fun, M &m);  
}

#endif
