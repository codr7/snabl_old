#ifndef SNABL_TYPES_MACRO_HPP
#define SNABL_TYPES_MACRO_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Macro: Type::Imp {
    Macro(Type::Id id, Sym name);
  };
}

#endif
