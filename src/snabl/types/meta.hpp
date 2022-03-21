#ifndef SNABL_TYPES_META_HPP
#define SNABL_TYPES_META_HPP

#include "snabl/type.hpp"

namespace snabl::types {
  struct Meta: Type::Imp {
    Meta(Type::Id id, Sym name);
  };
}

#endif
