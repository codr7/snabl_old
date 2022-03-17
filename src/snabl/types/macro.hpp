#ifndef SNABL_TYPES_MACRO_HPP
#define SNABL_TYPES_MACRO_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Macro: Type {
    struct Imp: Type::Imp {
      Imp(Id id, Sym name);
    };
    
    Macro(Lib &lib, Sym name);
  };
}

#endif
