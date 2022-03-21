#ifndef SNABL_TYPES_INT_HPP
#define SNABL_TYPES_INT_HPP

#include <cstdint>

#include "snabl/type.hpp"

namespace snabl::types {
  struct Int: Type::Imp {
    using DataType = int64_t;
    
    Int(Type::Id id, Sym name);
  };
}

#endif
