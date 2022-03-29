#include "snabl/m.hpp"
#include "snabl/type.hpp"
#include "snabl/types/sym.hpp"

namespace snabl::types {
  Sym::Sym(Type::Id id, snabl::Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) {
      out << '\'' << val.as<snabl::Sym>();
    };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      ops::LOAD_SYM(m.emit(), reg, val.as<snabl::Sym>());
      return nullopt;
    };

    methods.eq = [](Val left, Val right) { return left.as<snabl::Sym>() == right.as<snabl::Sym>(); };
  }
}
