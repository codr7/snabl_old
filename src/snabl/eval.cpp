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
      &&CALL, &&CALLI1, &&COPY, &&COPYS,
      &&DEC, &&EQ,
      &&FUN,
      &&GOTO,
      &&LOAD_BOOL, &&LOAD_FUN, &&LOAD_INT1, &&LOAD_INT2, &&LOAD_MACRO, &&LOAD_TYPE,
      &&MOVE, &&MOVES,
      &&NOP, &&ONE,
      &&REC, &&RET,
      &&STATE_BEG, &&STATE_END,
      &&TEST,
      &&Z,
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

  COPYS: {
      optional<Val> *rs = state->regs.begin(), *src = rs+ops::copy_src(op);
      copy(src, src+ops::copys_len(op), rs+ops::copy_dst(op));
      DISPATCH(pc+1);
    }

  DEC: {
      optional<Val> *rs = state->regs.begin(), &src = rs[ops::dec_src(op)];
      rs[ops::dec_dst(op)] = src = Val(abc_lib->int_type, src->as<types::Int::DataType>() - ops::dec_delta(op));
      DISPATCH(pc+1);
    }

  EQ: {
      optional<Val> *rs = state->regs.begin();

      {
	Val left = *rs[ops::eq_left(op)], right = *rs[ops::eq_right(op)];
	rs[ops::eq_dst(op)] = Val(abc_lib->bool_type, left == right);
      }
      
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

  LOAD_INT1: {
      state->regs[ops::load_reg(op)] = Val(abc_lib->int_type, ops::load_int1_val(op));
      DISPATCH(pc+1);
    }

  LOAD_INT2: {
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

  MOVES: {
      optional<Val> *rs = state->regs.begin(), *src = rs+ops::move_src(op);
      move(src, src+ops::copys_len(op), rs+ops::move_dst(op));
      DISPATCH(pc+1);
    }

  NOP: { DISPATCH(pc+1); }

  ONE: {
      optional<Val> *rs = state->regs.begin();
      rs[ops::one_dst(op)] = Val(abc_lib->bool_type, rs[ops::one_src(op)]->as<types::Int::DataType>() == 1);
      DISPATCH(pc+1);
    }

  REC: {
      State *prev = end_state();
      for (Reg i = 1; i < Fun::ARG_COUNT+1; i++) { state->regs[i] = move(prev->regs[i]); }
      deref_state(prev);
      DISPATCH(frame->target->start_pc);
    }
    
  RET: {
      Frame *f = end_frame();
      PC ret_pc = f->ret_pc;      
      ret_state(f->ret_reg);
      ret_state(f->ret_reg);
      deref_frame(f);
      DISPATCH(ret_pc);
    }

  STATE_BEG: {
      for (int i = 0; i < ops::state_beg_count(op); i++) {
	begin_state(ops::state_beg_reg_count(op));
      }
      
      DISPATCH(pc+1);
    }

  STATE_END: {
      ret_state(ops::state_end_reg(op));
      DISPATCH(pc+1);
    }

  TEST: {
      auto rs = state->regs.begin();
      cout << "Test " << *rs[ops::test_actual(op)] << " = " << *rs[ops::test_expected(op)] << "...";

      if (rs[ops::test_actual(op)] == rs[ops::test_expected(op)]) {
	rs[ops::test_result(op)] = Val(abc_lib->bool_type, true);
	cout << "OK" << endl;
      } else {
	rs[ops::test_result(op)] = Val(abc_lib->bool_type, false);
	cout << "FAIL" << endl;
      }
      
      DISPATCH(pc+1);
    }
    
  Z: {
      optional<Val> *rs = state->regs.begin();
      rs[ops::z_dst(op)] = Val(abc_lib->bool_type, rs[ops::z_src(op)]->as<types::Int::DataType>() == 0);
      DISPATCH(pc+1);
    }

    /* STOP */
    
  STOP: {}

    return nullopt;
  }
}

