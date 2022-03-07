#include <iostream>

#include "snabl/forms/id.hpp"

namespace snabl::forms {
  Id::Id(Pos pos, Sym name): Form::Imp(pos), name(name) {}

  void Id::dump(ostream &out) const { out << name; }
  
  optional<Error> Id::emit(Reg reg, M &m) const {
    return nullopt;
  }
}
