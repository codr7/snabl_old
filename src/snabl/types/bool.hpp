#ifndef SNABL_TYPES_BOOL_HPP
#define SNABL_TYPES_BOOL_HPP

#include <cstdint>

#include "snabl/type.hpp"

namespace snabl::types {
  struct Bool: Type::Imp {
    Bool(Type::Id id, Sym name);
  };
}

#endif
