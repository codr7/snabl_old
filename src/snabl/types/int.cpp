#include "snabl/m.hpp"
#include "snabl/types/int.hpp"

namespace snabl::types {
  Int::Imp::Imp(Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << val.as<DataType>(); };
    
    methods.emit = [](Val val, Reg reg, Pos pos, M &m) {
      ops::LOAD_INT(m.emit(2), reg, val.as<DataType>());
      return nullopt;
    };

    methods.is_true = [](Val val) { return val.as<DataType>() != 0; };
  }

  Int::Int(Lib &lib, Sym name): Type() {
    imp = make_shared<const Imp>(lib.add_type(*this), name);
  }
}
