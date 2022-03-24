#include "snabl/m.hpp"
#include "snabl/types/int.hpp"

namespace snabl::types {
  Int::Int(Type::Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) { out << val.as<DataType>(); };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      if (auto v = val.as<DataType>(); v >= LOAD_INT1_VAL_MIN && v <= LOAD_INT1_VAL_MAX) {
	ops::LOAD_INT1(m.emit(1), reg, v);
      } else {
	ops::LOAD_INT2(m.emit(2), reg, v);
      }

      return nullopt;
    };

    methods.eq = [](Val left, Val right) { return left.as<DataType>() == right.as<DataType>(); };

    methods.is_true = [](Val val) { return val.as<DataType>() != 0; };
  }
}
