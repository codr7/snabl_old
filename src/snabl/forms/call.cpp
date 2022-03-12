#include <iostream>

#include "snabl/m.hpp"
#include "snabl/forms/call.hpp"
#include "snabl/forms/id.hpp"

namespace snabl::forms {
  Call::Call(Pos pos, Form target, const deque<Form> &args): Form(make_shared<const Imp>(pos, target, args)) {}

  Call::Imp::Imp(Pos pos, Form target, const deque<Form> &args):
    Form::Imp(pos), target(target), args(args.begin(), args.end()) {}

  void Call::Imp::dump(ostream &out) const {
    out << '(';
    target.dump(out);

    for (Form f: args) {
      out << ' ';
      f.dump(out);
    }
    
    out << ')';
  }
  
  optional<Error> Call::Imp::emit(Reg reg, M &m) const {
    optional<Val> v(m.scope->get(target.as<Id>().name));
    if (!v) { return Error(pos, "Unknown call target: ", *v); }
    if (v->type != m.abc_lib->fun_type) { return Error(pos, "Invalid call target: ", *v); }
    ops::STATE(m.emit());

    for (int i = 0; i < args.size(); i++) {
      if (auto err = args[i].emit(i+1, m); err) { return err; }
    }
    
    Reg fun_reg = m.scope->reg_count++;
    ops::LOAD_FUN(m.emit(), fun_reg, v->as<Fun *>());
    ops::CALL(m.emit(), fun_reg, reg);
    return nullopt;
  }
}
