#include "snabl/fun.hpp"
#include "snabl/m.hpp"

#define DISPATCH(next_pc)						\
  goto *dispatch[static_cast<int>(op_code(op = ops[(pc = (next_pc))]))];

namespace snabl {
  using namespace std;
  
  optional<Error> M::eval(PC start_pc) {
    static const void* dispatch[] = {
      &&FUN, &&GOTO,
      &&LOAD_FUN, &&LOAD_INT, &&LOAD_TYPE,
      &&NOP, &&RET,
      /* STOP */
      &&STOP};

    PC pc = -1;
    Op op = 0;
    
    DISPATCH(start_pc);


  FUN: {
      Fun *f = env->regs[ops::fun_reg(op)].as<Fun *>();
      f->env = begin_env(env);
      DISPATCH(ops::fun_end_pc(op));
    }

  GOTO: {
      DISPATCH(ops::goto_pc(op));
    }

  LOAD_FUN: {
      Fun *f = reinterpret_cast<Fun *>(ops[pc+1]);
      env->regs[ops::load_reg(op)] = Val(abc_lib->fun_type, f);
      DISPATCH(pc+2);
    }

  LOAD_INT: {
      auto v = static_cast<types::Int::DataType>(ops[pc+1]);
      env->regs[ops::load_reg(op)] = Val(abc_lib->int_type, v);
      DISPATCH(pc+2);
    }

  LOAD_TYPE: {
      env->regs[ops::load_reg(op)] = Val(abc_lib->meta_type, types[ops::load_type_id(op)]);
      DISPATCH(pc+1);
    }

  NOP: {
      DISPATCH(pc+1);
    }
    
  RET: {
      Frame *f = end_frame();
      PC ret_pc = f->ret_pc;
      deref_frame(f);
      deref_env(end_env());
      DISPATCH(ret_pc);
    }
    
    /* STOP */
    
  STOP: {}

    return nullopt;
  }
}

