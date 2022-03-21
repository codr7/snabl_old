#ifndef SNABL_TYPES_FUN_HPP
#define SNABL_TYPES_FUN_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Fun: Type::Imp {
    Fun(Type::Id id, Sym name);
  };
}

#endif
