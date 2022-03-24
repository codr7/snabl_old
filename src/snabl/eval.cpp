#include <iostream>

#include "snabl/fun.hpp"
#include "snabl/m.hpp"
#include "snabl/timer.hpp"

#define DISPATCH(next_pc) {				\
    op = ops[(pc = (next_pc))];				\
							\
    if (debug) {					\
      cout << pc << ' ';				\
      op_dump(op, cout, *this);				\
      cout << endl;					\
    }							\
							\
    goto *dispatch[static_cast<int>(op_code(op))];	\
  }

namespace snabl {
  using namespace std;
  
  optional<Error> M::eval(PC start_pc) {
    static const void* dispatch[] = {
      &&BENCH, &&BRANCH,
      &&CALL, &&CALLI1, &&COPY,
      &&DEC, &&EQ, &&FUN, &&GOTO,
      &&LOAD_BOOL, &&LOAD_FUN, &&LOAD_INT, &&LOAD_MACRO, &&LOAD_TYPE,
      &&MOVE, &&NOP, &&RET, &&STATE,
      /* STOP */
      &&STOP};

    PC pc = -1;
    Op op = 0;
    
    DISPATCH(start_pc);

  BENCH: {
      optional<Val> &reg = *(state->regs.begin() + ops::bench_reg(op));
      auto reps = reg->as<types::Int::DataType>();

      Timer t;
      for (int i = 0; i < reps; i++) { eval(pc+1); }
      reg = Val(abc_lib->int_type, static_cast<types::Int::DataType>(t.ms()));
      DISPATCH(ops::bench_end(op));
    }
    
  BRANCH: {
      optional<Val> &c = state->regs[ops::branch_cond(op)];
      DISPATCH(c->is_true() ? pc+1 : ops::branch_else(op));
    }

  CALL: {
      Fun *target = state->regs[ops::call_target(op)]->as<Fun *>();
      Reg reg = ops::call_reg(op);
      PC ret_pc1 = pc+1;
      
      if (auto [ret_pc2, err] = target->call(reg, ret_pc1, *this); err) {
	return err;
      } else if (ret_pc2 == ret_pc1) {
	ret_state(reg);
      } else {
	ret_pc1 = ret_pc2;
      }
	  
      DISPATCH(ret_pc1); 
    }

  CALLI1: {
      Fun *target = ops::calli1_target(op);
      Reg reg = ops::calli1_reg(op);
      PC ret_pc1 = pc+1;
      
      if (auto [ret_pc2, err] = target->call(reg, ret_pc1, *this); err) {
	return err;
      } else if (ret_pc2 == ret_pc1) {
	ret_state(reg);
      } else {
	ret_pc1 = ret_pc2;
      }
	  
      DISPATCH(ret_pc1); 
    }

  COPY: {
      optional<Val> *rs = state->regs.begin();
      rs[ops::copy_dst(op)] = rs[ops::copy_src(op)];
      DISPATCH(pc+1);
    }

  DEC: {
      optional<Val> *rs = state->regs.begin(), &src = rs[ops::dec_src(op)];
      rs[ops::dec_dst(op)] = src = Val(abc_lib->int_type, src->as<types::Int::DataType>() - ops::dec_delta(op));
      DISPATCH(pc+1);
    }

  EQ: {
      optional<Val> *rs = state->regs.begin(), &left = rs[ops::eq_left(op)], &right = rs[ops::eq_right(op)];
      rs[ops::eq_dst(op)] = Val(abc_lib->bool_type, left->type.imp->methods.eq(*left, *right));
      DISPATCH(pc+1);
    }

  FUN: {
      Fun *f = state->regs[ops::fun_reg(op)]->as<Fun *>();
      f->state = begin_state(f->reg_count);
      end_state();
      DISPATCH(ops::fun_end(op));
    }

  GOTO: {
      DISPATCH(ops::goto_pc(op));
    }

  LOAD_BOOL: {
      state->regs[ops::load_reg(op)] = Val(abc_lib->bool_type, ops::load_bool_val(op));
      DISPATCH(pc+1);
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

  LOAD_MACRO: {
      Macro *m = reinterpret_cast<Macro *>(ops[pc+1]);
      state->regs[ops::load_reg(op)] = Val(abc_lib->macro_type, m);
      DISPATCH(pc+2);
    }

  LOAD_TYPE: {
      state->regs[ops::load_reg(op)] = Val(abc_lib->meta_type, types[ops::load_type_id(op)]);
      DISPATCH(pc+1);
    }

  MOVE: {
      optional<Val> *rs = state->regs.begin();
      rs[ops::move_dst(op)] = move(rs[ops::move_src(op)]);
      DISPATCH(pc+1);
    }

  NOP: { DISPATCH(pc+1); }
    
  RET: {
      Frame *f = end_frame();
      PC ret_pc = f->ret_pc;      
      ret_state(f->ret_reg);
      ret_state(f->ret_reg);
      deref_frame(f);
      DISPATCH(ret_pc);
    }

  STATE: {
      begin_state(ops::state_reg_count(op));
      DISPATCH(pc+1);
    }
    
    /* STOP */
    
  STOP: {}

    return nullopt;
  }
}

