#include "snabl/macro.hpp"
#include "snabl/m.hpp"
#include "snabl/types/macro.hpp"

namespace snabl::types {
  Macro::Imp::Imp(Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << val.as<snabl::Macro *>(); };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      ops::LOAD_MACRO(m.emit(2), reg, val.as<snabl::Macro *>());
      return nullopt;
    };
  }

  Macro::Macro(Lib &lib, Sym name): Type(lib, make_shared<const Imp>(lib.type_id(), name)) {}
}
