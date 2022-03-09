#include <iostream>

#include "snabl/m.hpp"
#include "snabl/forms/lit.hpp"

namespace snabl::forms {
  Lit::Lit(Pos pos, Type type, any data): Form(make_shared<const Imp>(pos, type, data)) {}

  Lit::Imp::Imp(Pos pos, Type type, any data): Form::Imp(pos), val(type, data) {}

  void Lit::Imp::dump(ostream &out) const { val.type.imp->methods.dump(val, out); }
  
  optional<Error> Lit::Imp::emit(Reg reg, M &m) const {
    return val.type.imp->methods.emit(val, reg, pos, m);
  }
}
