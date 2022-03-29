#include <iostream>

#include "snabl/form.hpp"
#include "snabl/fun.hpp"
#include "snabl/fuses/all.hpp"
#include "snabl/m.hpp"

namespace snabl {
  Fun::Arg::Arg() {}
  
  Fun::Arg::Arg(Sym name, Type type): name(name), type(type)  {}

  Fun::Fun(Sym name, const vector<Arg> &args, Type ret_type, Body body):
    name(name), arg_count(args.size()), ret_type(ret_type), body(body), reg_count(0), emit_reg(-1), start_pc(-1) {
    copy(args.begin(), args.end(), this->args.begin());
  }

  optional<Error> Fun::emit(deque<Form> body, Reg reg, M &m) {
    ops::LOAD_FUN(m.emit(2), reg, this);
    Op &op = m.emit();
    start_pc = m.emit_pc;
    reg_count = m.scope->reg_count;
    m.begin_scope();
    
    for (int i = 0; i < arg_count; i++) {
      m.scope->bind(args[i].name, m.abc_lib->reg_type, m.scope->reg_count + i);
    }

    ops::MOVES(m.emit(), m.scope->reg_count, Reg(1), arg_count);
    m.scope->reg_count += arg_count;
    emit_reg = reg;
    
    for (auto f: body) {
      if (optional<Error> err = f.emit(reg, m); err) { return err; }
    }

    m.deref_scope(m.end_scope());
    ops::RET(m.emit(), reg);
    ops::FUN(op, reg, m.emit_pc);
    fuses::all(this, m);

    this->body = [this](Fun &self, Reg ret_reg, PC ret_pc, M &m) {
      State *new_state = m.begin_state(args.size()+1);
      move(state->regs.begin()+ARG_COUNT+1, state->regs.begin()+reg_count, new_state->regs.begin()+ARG_COUNT+1);
      m.begin_frame(this, ret_reg, ret_pc);
      return pair<PC, optional<Error>>(start_pc, nullopt);
    };
      
    return nullopt;
  }

  pair<PC, optional<Error>> Fun::call(Reg reg, PC ret_pc, M &m) {
    return body(*this, reg, ret_pc, m);
  }

  ostream &operator <<(ostream &out, Fun *val) {
    out << "(Fun " << val->name << ')';
    return out;
  }
}
