#include "snabl/macro.hpp"
#include "snabl/m.hpp"
#include "snabl/types/macro.hpp"

namespace snabl::types {
  Macro::Macro(Type::Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << val.as<snabl::Macro *>(); };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      ops::LOAD_MACRO(m.emit(2), reg, val.as<snabl::Macro *>());
      return nullopt;
    };

    methods.eq = [](Val left, Val right) { return left.as<snabl::Macro *>() == right.as<snabl::Macro *>(); };
  }
}
