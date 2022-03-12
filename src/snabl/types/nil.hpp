#ifndef SNABL_TYPES_NIL_HPP
#define SNABL_TYPES_NIL_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Nil: Type {
    struct Imp: Type::Imp {
      Imp(Id id, Sym name);
    };
    
    Nil(Lib &lib, Sym name);
  };
}

#endif
