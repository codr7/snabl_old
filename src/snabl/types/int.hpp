#ifndef SNABL_TYPES_INT_HPP
#define SNABL_TYPES_INT_HPP

#include <cstdint>

#include "snabl/type.hpp"

namespace snabl::types {
  struct Int: Type {
    using DataType = int64_t;
    
    struct Imp: Type::Imp {
      Imp(Id id, Sym name);
    };
    
    Int(Lib &lib, Sym name);
  };
}

#endif
