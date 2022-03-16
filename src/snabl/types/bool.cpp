#include "snabl/m.hpp"
#include "snabl/types/int.hpp"

namespace snabl::types {
  Bool::Imp::Imp(Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << (val.as<bool>() ? 'T' : 'F'); };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      ops::LOAD_BOOL(m.emit(), reg, val.as<bool>());
      return nullopt;
    };

    methods.is_true = [](Val val) { return val.as<bool>(); };
  }

  Bool::Bool(Lib &lib, Sym name): Type(lib, make_shared<const Imp>(lib.type_id(), name)) {}
}
