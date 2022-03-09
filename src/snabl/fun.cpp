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
      m.scope->set(a.name, m.abc_lib->reg_type, reg);
      ops::COPY(m.emit(), reg, Reg(i+1));
    }

    if (optional<Error> err = body.emit(0, m); err) { return err; }
    ops::RET(m.emit());
    ops::FUN(op, fun_reg, m.emit_pc);

    this->body = [start_pc](Fun &self, PC ret_pc, M &m) {
      Env *prenv = m.env;
      Env *env = m.begin_env(self.env);
      copy(prenv->regs.begin()+1, prenv->regs.begin()+ARG_COUNT+1, env->regs.begin()+1);
      m.begin_frame(ret_pc);
      return pair<PC, optional<Error>>(start_pc, nullopt);
    };
      
    return nullopt;
  }

  pair<PC, optional<Error>> Fun::call(PC ret_pc, M &m) {
    return body(*this, ret_pc, m);
  }

  ostream &operator <<(ostream &out, Fun *val) {
    out << val->name << "()";
    return out;
  }
}
