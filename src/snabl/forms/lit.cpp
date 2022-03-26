#include <iostream>

#include "snabl/m.hpp"
#include "snabl/forms/lit.hpp"

namespace snabl::forms {
  Lit::Lit(Pos pos, Type type, any data): Form(make_shared<const Imp>(pos, type, data)) {}

  Lit::Imp::Imp(Pos pos, Type type, any data): Form::Imp(pos), _val(type, data) {}

  void Lit::Imp::dump(ostream &out) const { _val.type.imp->methods.dump(_val, out); }
  
  optional<Error> Lit::Imp::emit(Reg reg, M &m) const {
    return _val.type.imp->methods.emit(_val, reg, pos, m);
  }

  optional<Val> Lit::Imp::val(M &m) const { return _val; }
}
