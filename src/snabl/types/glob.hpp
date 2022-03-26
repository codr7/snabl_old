#ifndef SNABL_TYPES_GLOB_HPP
#define SNABL_TYPES_GLOB_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Glob: Type::Imp {
    Glob(Type::Id id, Sym name);
  };
}

#endif
