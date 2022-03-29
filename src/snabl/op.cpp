#include "snabl/m.hpp"
#include "snabl/op.hpp"

namespace snabl {
  PC op_len(Op op) {
    switch (op_code(op)) {
    case OpCode::BENCH: case OpCode::BRANCH:
    case OpCode::CALLI1: case OpCode::COPY: case OpCode::COPYS:
    case OpCode::DEC:
    case OpCode::EQ:
    case OpCode::FUN:
    case OpCode::GOTO:
    case OpCode::LOAD_BOOL: case OpCode::LOAD_INT1: case OpCode::LOAD_TYPE:
    case OpCode::MOVE: case OpCode::MOVES:
    case OpCode::NOP:
    case OpCode::ONE:
    case OpCode::REC: case OpCode::RET:
    case OpCode::STATE_BEG: case OpCode::STATE_END: case OpCode::STOP:
    case OpCode::TEST: case OpCode::TRACE:
    case OpCode::Z:
      break;
    case OpCode::CALL:
    case OpCode::LOAD_FUN: case OpCode::LOAD_INT2: case OpCode::LOAD_MACRO:
      return 2;
    }

    return 1;
  }

  bool op_reads(Op op, Reg reg) {
    switch (op_code(op)) {
    case OpCode::GOTO:
    case OpCode::LOAD_BOOL:
    case OpCode::LOAD_FUN:
    case OpCode::LOAD_INT1:
    case OpCode::LOAD_INT2:
    case OpCode::LOAD_MACRO:
    case OpCode::LOAD_TYPE:
    case OpCode::NOP:
    case OpCode::TRACE:
    case OpCode::STATE_BEG:
    case OpCode::STATE_END:
    case OpCode::STOP:
      break;
    case OpCode::BENCH:
      return reg == ops::bench_reps(op);
    case OpCode::BRANCH:
      return reg == ops::branch_cond(op);
    case OpCode::CALLI1:
    case OpCode::REC:
      return (reg >= 1 && reg < Fun::ARG_COUNT+1);
    case OpCode::CALL:
      return (reg >= 1 && reg < Fun::ARG_COUNT+1) || (reg == ops::call_target(op));
    case OpCode::COPY:
      return reg == ops::copy_src(op);
    case OpCode::COPYS:
      return reg >= ops::copy_src(op) && reg < (ops::copy_src(op) + ops::copys_len(op));
    case OpCode::DEC:
      return reg == ops::dec_src(op);
    case OpCode::EQ:
      return reg == ops::eq_left(op) || reg == ops::eq_right(op);
    case OpCode::FUN:
      return reg == ops::fun_reg(op);
    case OpCode::MOVE:
      return reg == ops::move_src(op);
    case OpCode::MOVES:
      return reg >= ops::move_src(op) && reg < (ops::move_src(op) + ops::moves_len(op));
    case OpCode::ONE:
      return reg == ops::one_src(op);
    case OpCode::RET:
      return reg == ops::ret_reg(op);
    case OpCode::TEST:
      return reg == ops::test_expected(op) || reg == ops::test_actual(op);
    case OpCode::Z:
      return reg == ops::z_src(op);
    }
    
    return false;
  }

  bool op_writes(Op op, Reg reg) {
    switch (op_code(op)) {
    case OpCode::GOTO:
    case OpCode::NOP:
    case OpCode::REC:
    case OpCode::STOP:
    case OpCode::STATE_BEG:
    case OpCode::STATE_END:
    case OpCode::TRACE:
      break;
    case OpCode::BENCH:
      return reg == ops::bench_res(op);
    case OpCode::CALLI1:
      return reg == ops::calli1_reg(op);
    case OpCode::LOAD_BOOL:
    case OpCode::LOAD_FUN:
    case OpCode::LOAD_INT1:
    case OpCode::LOAD_INT2:
    case OpCode::LOAD_MACRO:
    case OpCode::LOAD_TYPE:
      return reg == ops::load_dst(op);
    case OpCode::BRANCH:
      return reg == ops::branch_reg(op);
    case OpCode::CALL:
      return reg == ops::call_reg(op);
    case OpCode::COPY:
      return reg == ops::copy_dst(op);
    case OpCode::COPYS:
      return reg >= ops::copy_dst(op) && reg < (ops::copy_dst(op) + ops::copys_len(op));
    case OpCode::DEC:
      return reg == ops::dec_dst(op);
    case OpCode::EQ:
      return reg == ops::eq_dst(op);
    case OpCode::FUN:
      return reg == ops::fun_reg(op);
    case OpCode::MOVE:
      return reg == ops::move_dst(op);
    case OpCode::MOVES:
      return reg >= ops::move_dst(op) && reg < (ops::move_dst(op) + ops::moves_len(op));
    case OpCode::ONE:
      return reg == ops::one_dst(op);
    case OpCode::RET:
      return reg == ops::ret_reg(op);
    case OpCode::TEST:
      return reg == ops::test_result(op);
    case OpCode::Z:
      return reg == ops::z_dst(op);
    }
    
    return false;
  }

  void op_trace(PC pc, ostream &out, M &m) {
    out << pc << ' ';					\
    Op op = m.ops[pc];
    
    switch (op_code(op)) {
    case OpCode::BENCH:
      out << "BENCH " << ops::bench_reps(op) << ' ' << ops::bench_res(op) << ' ' << ops::bench_end(op);
      break;
    case OpCode::BRANCH:
      out << "BRANCH " <<
	ops::branch_cond(op) << ' ' <<
	ops::branch_reg(op) << ' ' <<
	ops::branch_if_pc(op) << ' ' <<
	ops::branch_else_pc(op);
      
      break;
    case OpCode::CALL:
      out << "CALL " << ops::call_target(op) << ' ' << ops::call_reg(op);
      break;
    case OpCode::CALLI1:
      out << "CALLI1 " << ops::calli1_reg(op) << ' ' << ops::calli1_target(op);
      break;
    case OpCode::COPY:
      out << "COPY " << ops::copy_dst(op) << ' ' << ops::copy_src(op);
      break;
    case OpCode::COPYS:
      out << "COPYS " << ops::copy_dst(op) << ' ' << ops::copy_src(op) << ' ' << ops::copys_len(op);
      break;
    case OpCode::DEC:
      out << "DEC " << ops::dec_dst(op) << ' ' << ops::dec_src(op) << ' ' << ops::dec_delta(op);
      break;
    case OpCode::EQ:
      out << "EQ " << ops::eq_dst(op) << ' ' << ops::eq_left(op) << ' ' << ops::eq_right(op);
      break;
    case OpCode::FUN:
      out << "FUN " << ops::fun_reg(op) << ' ' << ops::fun_end(op);
      break;
    case OpCode::GOTO:
      out << "GOTO " << ops::goto_pc(op);
      break;
    case OpCode::LOAD_BOOL:
      out << "LOAD_BOOL " << ops::load_bool_val(op);
      break;
    case OpCode::LOAD_FUN:
      out << "LOAD_FUN " << ops::load_dst(op);
      break;
    case OpCode::LOAD_INT1:
      out << "LOAD_INT1 " << ops::load_dst(op) << ' ' << ops::load_int1_val(op);
      break;
    case OpCode::LOAD_INT2:
      out << "LOAD_INT2 " << ops::load_dst(op);
      break;
    case OpCode::LOAD_MACRO:
      out << "LOAD_MACRO " << ops::load_dst(op);
      break;
    case OpCode::LOAD_TYPE:
      out << "LOAD_TYPE " << ops::load_dst(op);
      break;
    case OpCode::MOVE:
      out << "MOVE " << ops::move_dst(op) << ' ' << ops::move_src(op);
      break;
    case OpCode::MOVES:
      out << "MOVES " << ops::move_dst(op) << ' ' << ops::move_src(op) << ' ' << ops::moves_len(op);
      break;
    case OpCode::NOP:
      out << "NOP";
      break;
    case OpCode::REC:
      out << "REC";
      break;
    case OpCode::ONE:
      out << "ONE " << ops::one_dst(op) << ' ' << ops::one_src(op);
      break;
    case OpCode::RET:
      out << "RET " << ops::ret_reg(op);
      break;
    case OpCode::STATE_BEG:
      out << "STATE_BEG " << ops::state_beg_count(op);
      break;
    case OpCode::STATE_END:
      out << "STATE_END";
      break;
    case OpCode::TEST:
      out << "TEST " <<
	ops::test_expected(op) << ' ' <<
	ops::test_actual(op) << ' ' <<
	ops::test_result(op);      
      break;
    case OpCode::TRACE:
      out << "TRACE";
      break;
    case OpCode::Z:
      out << "Z " << ops::z_dst(op) << ' ' << ops::z_src(op);
      break;
    case OpCode::STOP:
      out << "STOP";
      break;
    }

    out << endl;
  }

  namespace ops {    
    void BENCH(Op &op, Reg reps, Reg res, PC end_pc) {
      op = static_cast<Op>(static_cast<Op>(OpCode::BENCH) +
			   (reps << BENCH_REPS_BIT) +
			   (res << BENCH_RES_BIT) +
			   (end_pc << BENCH_END_BIT));
    }
    
    void BRANCH(Op &op, Reg cond, Reg reg, PC if_pc, PC else_pc) {
      op = static_cast<Op>(static_cast<Op>(OpCode::BRANCH) +
			   (cond << BRANCH_COND_BIT) +
			   (reg << BRANCH_REG_BIT) +
			   (if_pc << BRANCH_IF_BIT) +
			   (else_pc << BRANCH_ELSE_BIT));
    }

    void CALL(Op &op, Reg target, Reg reg) {
      op = static_cast<Op>(static_cast<Op>(OpCode::CALL) +
			   (target << CALL_TARGET_BIT) +
			   (reg << CALL_REG_BIT));
    }

    void CALLI1(Op &op, Reg reg, Fun *target) {
      op = static_cast<Op>(static_cast<Op>(OpCode::CALLI1) +
			   (reg << CALLI1_REG_BIT) +
			   (reinterpret_cast<Op>(target) << CALLI1_TARGET_BIT));
    }

    void COPY(Op &op, Reg dst, Reg src) {
      op = static_cast<Op>(static_cast<Op>(OpCode::COPY) + (dst << COPY_DST_BIT) + (src << COPY_SRC_BIT));
    }

    void COPYS(Op &op, Reg dst, Reg src, int len) {
      op = static_cast<Op>(static_cast<Op>(OpCode::COPYS) +
			   (dst << COPY_DST_BIT) +
			   (src << COPY_SRC_BIT) +
			   (len << COPYS_LEN_BIT));
    }

    void DEC(Op &op, Reg dst, Reg src, types::Int::DataType delta) {
      op = static_cast<Op>(static_cast<Op>(OpCode::DEC) +
			   (dst << DEC_DST_BIT) +
			   (src << DEC_SRC_BIT) +
			   (delta << DEC_DELTA_BIT));
    }

    void EQ(Op &op, Reg dst, Reg left, Reg right) {
      op = static_cast<Op>(static_cast<Op>(OpCode::EQ) +
			   (dst << EQ_DST_BIT) +
			   (left << EQ_LEFT_BIT) +
			   (right << EQ_RIGHT_BIT));
    }

    void FUN(Op &op, Reg reg, PC end) {
      op = static_cast<Op>(static_cast<Op>(OpCode::FUN) + (reg << OP_CODE_BITS) + (end << FUN_END_BIT));
    }
        
    void GOTO(Op &op, PC pc) { op = static_cast<Op>(static_cast<Op>(OpCode::GOTO) + (pc << OP_CODE_BITS)); }
    
    void LOAD_BOOL(Op &op, Reg reg, bool val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_BOOL) +
			   (reg << LOAD_DST_BIT) +
			   ((val ? 1 : 0) << LOAD_VAL_BIT));
    }
    
    void LOAD_FUN(Op &op, Reg reg, snabl::Fun *val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_FUN) + (reg << OP_CODE_BITS));
      *(&op+1) = reinterpret_cast<Op>(val);
    }

    void LOAD_INT(Op &op, Reg reg, snabl::types::Int::DataType val) {
    }

    void LOAD_INT1(Op &op, Reg reg, snabl::types::Int::DataType val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_INT1) + (reg << LOAD_DST_BIT) + (val << LOAD_VAL_BIT));
    }

    void LOAD_INT2(Op &op, Reg reg, snabl::types::Int::DataType val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_INT2) + (reg << LOAD_DST_BIT));
      *(&op+1) = static_cast<Op>(val);
    }

    void LOAD_MACRO(Op &op, Reg reg, snabl::Macro *val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_MACRO) + (reg << LOAD_DST_BIT));
      *(&op+1) = reinterpret_cast<Op>(val);
    }

    void LOAD_TYPE(Op &op, Reg reg, snabl::Type val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_TYPE) +
			   (reg << LOAD_DST_BIT) +
			   (val.imp->id << LOAD_VAL_BIT));
    }

    void MOVE(Op &op, Reg dst, Reg src) {
      op = static_cast<Op>(static_cast<Op>(OpCode::MOVE) + (dst << MOVE_DST_BIT) + (src << MOVE_SRC_BIT));
    }

    void MOVES(Op &op, Reg dst, Reg src, int len) {
      op = static_cast<Op>(static_cast<Op>(OpCode::MOVES) +
			   (dst << MOVE_DST_BIT) +
			   (src << MOVE_SRC_BIT) +
			   (len << MOVES_LEN_BIT));
    }
    
    void NOP(Op &op) { op = static_cast<Op>(OpCode::NOP); }
    void REC(Op &op) { op = static_cast<Op>(OpCode::REC); }

    void ONE(Op &op, Reg dst, Reg src) {
      op = static_cast<Op>(static_cast<Op>(OpCode::ONE) + (dst << ONE_DST_BIT) + (src << ONE_SRC_BIT));
    }

    void RET(Op &op, Reg reg) { op = static_cast<Op>(OpCode::RET) + (reg << RET_REG_BIT); }

    void STATE_BEG(Op &op, int count) {
      op = static_cast<Op>(static_cast<Op>(OpCode::STATE_BEG) +
			   (count << STATE_BEG_COUNT_BIT));
    }

    void STATE_END(Op &op, Reg reg) {
      op = static_cast<Op>(static_cast<Op>(OpCode::STATE_END) + (reg << STATE_END_REG_BIT));
    }

    void TEST(Op &op, Reg expected, Reg actual, Reg result) {
      op = static_cast<Op>(static_cast<Op>(OpCode::TEST) +
			   (expected << TEST_EXPECTED_BIT) +
			   (actual << TEST_ACTUAL_BIT) +
			   (result << TEST_RESULT_BIT));
    }

    void TRACE(Op &op) { op = static_cast<Op>(OpCode::TRACE); }

    void Z(Op &op, Reg dst, Reg src) {
      op = static_cast<Op>(static_cast<Op>(OpCode::Z) + (dst << Z_DST_BIT) + (src << Z_SRC_BIT));
    }

    void STOP(Op &op) { op = static_cast<Op>(OpCode::STOP); }
  }
}
