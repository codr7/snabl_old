#ifndef SNABL_TYPES_NIL_HPP
#define SNABL_TYPES_NIL_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Nil: Type::Imp {
    Nil(Type::Id id, Sym name);
  };
}

#endif
