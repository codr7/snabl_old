#include <iostream>

#include "snabl/fun.hpp"
#include "snabl/m.hpp"

#define DISPATCH(next_pc) {						\
    op = ops[(pc = (next_pc))];						\
									\
    if (debug) {							\
      cout << pc << ' ';						\
      op_dump(op, cout);						\
      cout << endl;							\
    }									\
									\
    goto *dispatch[static_cast<int>(op_code(op))];			\
  }

namespace snabl {
  using namespace std;
  
  optional<Error> M::eval(PC start_pc) {
    static const void* dispatch[] = {
      &&CALL, &&COPY, &&FUN, &&GOTO,
      &&LOAD_FUN, &&LOAD_INT, &&LOAD_TYPE,
      &&NOP, &&RET,
      /* STOP */
      &&STOP};

    PC pc = -1;
    Op op = 0;
    
    DISPATCH(start_pc);
    
  CALL: {
      Fun *target = state->regs[ops::call_target(op)].as<Fun *>();
      Reg reg = ops::call_reg(op);
      PC ret_pc = pc+1;

      {
	auto [ret_pc2, err] = target->call(reg, ret_pc, *this);
	if (err) { return err; }
	
	if (ret_pc2 == ret_pc) {
	  State *s = end_state();
	  state->regs[reg] = s->regs[reg];
	  deref_state(s);
	}

	ret_pc = ret_pc2;
      }
	  
      DISPATCH(ret_pc); 
    }
    
  COPY: {
      state->regs[ops::copy_dst(op)] = state->regs[ops::copy_src(op)];
      DISPATCH(pc+1);
    }

  FUN: {
      Fun *f = state->regs[ops::fun_reg(op)].as<Fun *>();
      f->state = begin_state(state);
      DISPATCH(ops::fun_end_pc(op));
    }

  GOTO: {
      DISPATCH(ops::goto_pc(op));
    }

  LOAD_FUN: {
      Fun *f = reinterpret_cast<Fun *>(ops[pc+1]);
      state->regs[ops::load_reg(op)] = Val(abc_lib->fun_type, f);
      DISPATCH(pc+2);
    }

  LOAD_INT: {
      auto v = static_cast<types::Int::DataType>(ops[pc+1]);
      state->regs[ops::load_reg(op)] = Val(abc_lib->int_type, v);
      DISPATCH(pc+2);
    }

  LOAD_TYPE: {
      state->regs[ops::load_reg(op)] = Val(abc_lib->meta_type, types[ops::load_type_id(op)]);
      DISPATCH(pc+1);
    }

  NOP: {
      DISPATCH(pc+1);
    }
    
  RET: {
      Frame *f = end_frame();
      PC ret_pc = f->ret_pc;
      deref_frame(f);
      deref_state(end_state());
      DISPATCH(ret_pc);
    }
    
    /* STOP */
    
  STOP: {}

    return nullopt;
  }
}

