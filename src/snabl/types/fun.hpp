#ifndef SNABL_TYPES_FUN_HPP
#define SNABL_TYPES_FUN_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Fun: Type {
    struct Imp: Type::Imp {
      Imp(Id id, Sym name);
    };
    
    Fun(M &m, Sym name);
  };
}

#endif
