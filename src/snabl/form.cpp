#include "snabl/form.hpp"

namespace snabl {
  Form::Imp::Imp(Pos pos): pos(pos) {}

  Form::Imp::~Imp() {}

  Form::Form(shared_ptr<const Imp> imp): imp(imp) {}

  void Form::dump(ostream& out) const { imp->dump(out); }

  optional<Error> Form::emit(Reg reg, M &m) const { return imp->emit(reg, m); }
}
