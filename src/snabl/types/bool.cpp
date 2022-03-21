#include "snabl/m.hpp"
#include "snabl/types/bool.hpp"

namespace snabl::types {
  Bool::Bool(Type::Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << (val.as<bool>() ? 'T' : 'F'); };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      ops::LOAD_BOOL(m.emit(), reg, val.as<bool>());
      return nullopt;
    };

    methods.is_true = [](Val val) { return val.as<bool>(); };
  }
}
