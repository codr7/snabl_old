#ifndef SNABL_TYPES_SYM_HPP
#define SNABL_TYPES_SYM_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Sym: Type::Imp {
    Sym(Type::Id id, snabl::Sym name);
  };
}

#endif
