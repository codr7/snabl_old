#ifndef SNABL_TYPES_META_HPP
#define SNABL_TYPES_META_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Meta: Type {
    struct Imp: Type::Imp {
      Imp(Id id, Sym name);
    };
    
    Meta(Lib &lib, Sym name);
  };
}

#endif
