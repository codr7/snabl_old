#ifndef SNABL_TYPES_REG_HPP
#define SNABL_TYPES_REG_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Reg: Type::Imp {
    Reg(Type::Id id, Sym name);
  };
}

#endif
