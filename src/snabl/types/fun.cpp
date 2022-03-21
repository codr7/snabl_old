#include "snabl/fun.hpp"
#include "snabl/m.hpp"
#include "snabl/types/fun.hpp"

namespace snabl::types {
  Fun::Fun(Type::Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << val.as<snabl::Fun *>(); };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      ops::LOAD_FUN(m.emit(2), reg, val.as<snabl::Fun *>());
      return nullopt;
    };
  }
}
