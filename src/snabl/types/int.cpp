#include "snabl/m.hpp"
#include "snabl/types/int.hpp"

namespace snabl::types {
  Int::Int(Type::Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << val.as<DataType>(); };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      ops::LOAD_INT(m.emit(2), reg, val.as<DataType>());
      return nullopt;
    };

    methods.is_true = [](Val val) { return val.as<DataType>() != 0; };
  }
}
