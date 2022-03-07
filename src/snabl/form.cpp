#include "snabl/form.hpp"

namespace snabl {
  Form::Imp::Imp(Pos pos): pos(pos) {}

  Form::Form(shared_ptr<const Imp> imp): imp(imp) {}

  void Form::dump(ostream& out) {
    imp->dump(out);
  }

  optional<Error> Form::emit(Reg reg, M &m) {
    return imp->emit(reg, m);
  }
}
