#ifndef SNABL_TYPES_BOOL_HPP
#define SNABL_TYPES_BOOL_HPP

#include <cstdint>

#include "snabl/type.hpp"

namespace snabl::types {
  struct Bool: Type {
    struct Imp: Type::Imp {
      Imp(Id id, Sym name);
    };
    
    Bool(Lib &lib, Sym name);
  };
}

#endif
