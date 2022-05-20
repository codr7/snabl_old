#include "snabl/form.hpp"

namespace snabl {
  Form::Imp::Imp(Pos pos): pos(pos) {}
  Form::Imp::~Imp() {}

  optional<Val> Form::Imp::val(M &m) const { return nullopt; }
  
  optional<Error> Form::Imp::quote(Reg reg, M &m) const { return emit(reg, m); }

  Form::Form(shared_ptr<const Imp> imp): imp(imp) {}

  void Form::dump(ostream& out) const { imp->dump(out); }

  optional<Error> Form::emit(Reg reg, M &m) const { return imp->emit(reg, m); }

  optional<Error> Form::quote(Reg reg, M &m) const { return imp->quote(reg, m); }

  optional<Val> Form::val(M &m) const { return imp->val(m); }

  ostream &operator<<(ostream &out, Form val) {
    val.dump(out);
    return out;
  }
}
