#include <iostream>

#include "snabl/m.hpp"
#include "snabl/macro.hpp"
#include "snabl/forms/slice.hpp"
#include "snabl/forms/id.hpp"

namespace snabl::forms {
  Slice::Slice(Pos pos, const deque<Form> &items): Form(make_shared<const Imp>(pos, items)) {}

  Slice::Imp::Imp(Pos pos, const deque<Form> &items): Form::Imp(pos), items(items) {}

  void Slice::Imp::dump(ostream &out) const {
    out << '[';

    for (Form f: items) {
      out << ' ';
      f.dump(out);
    }
    
    out << ']';
  }
  
  optional<Error> Slice::Imp::emit(Reg reg, M &m) const {
    return Error(pos, "Slice emit not implemented!");
  }

  bool Slice::Imp::is_atom() const { return false; }
}
