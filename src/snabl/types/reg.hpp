#ifndef SNABL_TYPES_REG_HPP
#define SNABL_TYPES_REG_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Reg: Type {
    struct Imp: Type::Imp {
      Imp(Id id, Sym name);
    };
    
    Reg(Lib &lib, Sym name);
  };
}

#endif
