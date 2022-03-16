#include "snabl/m.hpp"
#include "snabl/type.hpp"
#include "snabl/types/meta.hpp"

namespace snabl::types {
  Meta::Imp::Imp(Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) {
      out << val.as<snabl::Type>();
    };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      ops::LOAD_TYPE(m.emit(), reg, val.as<snabl::Type>());
      return nullopt;
    };
  }

  Meta::Meta(Lib &lib, Sym name): Type(lib, make_shared<const Imp>(lib.type_id(), name)) {}
}
