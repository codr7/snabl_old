#include "snabl/m.hpp"
#include "snabl/types/reg.hpp"

namespace snabl::types {
  Reg::Reg(Type::Id id, Sym name): Type::Imp(id, name) {
    methods.dump = [](Val val, ostream &out) {
      out << '#' << val.as<snabl::Reg>();
    };
    
    methods.emit = [](Val val, snabl::Reg reg, Pos pos, M &m) {
      if (snabl::Reg src = val.as<snabl::Reg>(); src != reg) { ops::COPY(m.emit(), reg, src); }
      return nullopt;
    };

    methods.eq = [](Val left, Val right) { return left.as<snabl::Reg>() == right.as<snabl::Reg>(); };
  }
}
