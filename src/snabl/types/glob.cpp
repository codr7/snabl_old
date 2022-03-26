#include "snabl/m.hpp"
#include "snabl/types/glob.hpp"

namespace snabl::types {
  Glob::Glob(Type::Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << val.as<snabl::Glob>(); };
    
    methods.emit = [](Val val, Reg reg, Pos pos, M &m) {
      Glob* g = val.as<Glob *>();
      return g.type.imp->methods.emit(val, reg, pos, m);
    };

    methods.eq = [](Val left, Val right) { return left.as<snabl::Glob>() == right.as<snabl::Glob>(); };
  }
}
