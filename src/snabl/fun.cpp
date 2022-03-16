#include "snabl/form.hpp"
#include "snabl/fun.hpp"
#include "snabl/m.hpp"

namespace snabl {
  Fun::Fun(Sym name, const vector<Arg> &args, Type ret_type, Body body):
    name(name), arg_count(args.size()), ret_type(ret_type), body(body) {
    copy(args.begin(), args.end(), this->args.begin());
  }

  optional<Error> Fun::emit(Form body, M &m) {
    Reg fun_reg = m.scope->reg_count++;
    ops::LOAD_FUN(m.emit(2), fun_reg, this);
    Op &op = m.emit();
    PC start_pc = m.emit_pc;

    for (int i = 0; i < arg_count; i++) {
      Arg &a = args[i];
      Reg reg = m.scope->reg_count++;
      m.scope->bind(a.name, m.abc_lib->reg_type, reg);
      ops::COPY(m.emit(), reg, Reg(i+1));
    }

    if (optional<Error> err = body.emit(0, m); err) { return err; }
    ops::RET(m.emit());
    ops::FUN(op, fun_reg, m.emit_pc);

    this->body = [start_pc](Fun &self, Reg ret_reg, PC ret_pc, M &m) {
      State *prstate = m.state;
      State *state = m.begin_state(self.state);
      copy(prstate->regs.begin()+1, prstate->regs.begin()+ARG_COUNT+1, state->regs.begin()+1);
      m.begin_frame(ret_reg, ret_pc);
      return pair<PC, optional<Error>>(start_pc, nullopt);
    };
      
    return nullopt;
  }

  pair<PC, optional<Error>> Fun::call(Reg reg, PC ret_pc, M &m) {
    return body(*this, reg, ret_pc, m);
  }

  ostream &operator <<(ostream &out, Fun *val) {
    out << val->name << "()";
    return out;
  }
}
