#include "snabl/m.hpp"
#include "snabl/types/int.hpp"

namespace snabl::types {
  Nil::Imp::Imp(Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << '_'; };

    /*
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      ops::LOAD_NIL(reg);
      return nullopt;
      };*/

    methods.is_true = [](Val val) { return false; };
  }

  Nil::Nil(Lib &lib, Sym name): Type(lib, make_shared<const Imp>(lib.type_id(), name)) {}
}
