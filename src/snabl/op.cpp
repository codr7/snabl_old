#include "snabl/m.hpp"
#include "snabl/op.hpp"

namespace snabl {
   PC op_len(Op op) {
    switch (op_code(op)) {
    case OpCode::BENCH: case OpCode::BRANCH:
    case OpCode::CALLI1: case OpCode::COPY:
    case OpCode::DEC:
    case OpCode::EQ:
    case OpCode::FENCE:
    case OpCode::FUN:
    case OpCode::GOTO:
    case OpCode::LOAD_BOOL: case OpCode::LOAD_INT1: case OpCode::LOAD_TYPE:
    case OpCode::MOVE:
    case OpCode::NOP:
    case OpCode::REC: case OpCode::RET:
    case OpCode::STATE: case OpCode::STOP:
    case OpCode::Z:
      break;
    case OpCode::CALL:
    case OpCode::LOAD_FUN: case OpCode::LOAD_INT2: case OpCode::LOAD_MACRO:
      return 2;
    }

    return 1;
  }

  void op_dump(Op op, ostream &out, M &m) {
    switch (op_code(op)) {
    case OpCode::BENCH:
      out << "BENCH " << ops::bench_reg(op) << ' ' << ops::bench_end(op);
      break;
    case OpCode::BRANCH:
      out << "BRANCH " << ops::branch_cond(op) << ' ' << ops::branch_reg(op) << ' ' << ops::branch_else(op);
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
    case OpCode::DEC:
      out << "DEC " << ops::dec_dst(op) << ' ' << ops::dec_src(op) << ' ' << ops::dec_delta(op);
      break;
    case OpCode::EQ:
      out << "EQ " << ops::eq_dst(op) << ' ' << ops::eq_left(op) << ' ' << ops::eq_right(op);
      break;
    case OpCode::FENCE:
      out << "FENCE";
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
      out << "LOAD_FUN " << ops::load_reg(op);
      break;
    case OpCode::LOAD_INT1:
      out << "LOAD_INT1 " << ops::load_reg(op) << ' ' << ops::load_int1_val(op);
      break;
    case OpCode::LOAD_INT2:
      out << "LOAD_INT2 " << ops::load_reg(op);
      break;
    case OpCode::LOAD_MACRO:
      out << "LOAD_MACRO " << ops::load_reg(op);
      break;
    case OpCode::LOAD_TYPE:
      out << "LOAD_TYPE " << ops::load_reg(op);
      break;
    case OpCode::MOVE:
      out << "MOVE " << ops::move_dst(op) << ' ' << ops::move_src(op);
      break;
    case OpCode::NOP:
      out << "NOP";
      break;
    case OpCode::REC:
      out << "REC";
      break;
    case OpCode::RET:
      out << "RET " << m.frame->ret_reg;
      break;
    case OpCode::STATE:
      out << "STATE " << ops::state_reg_count(op);
      break;
    case OpCode::Z:
      out << "Z " << ops::z_dst(op) << ' ' << ops::z_src(op);
      break;
    case OpCode::STOP:
      out << "STOP";
      break;
    }
  }

  namespace ops {    
    void BENCH(Op &op, Reg reg, PC end_pc) {
      op = static_cast<Op>(static_cast<Op>(OpCode::BENCH) +
			   (reg << BENCH_REG_BIT) +
			   (end_pc << BENCH_END_BIT));
    }
    
    void BRANCH(Op &op, Reg cond, Reg reg, PC else_pc) {
      op = static_cast<Op>(static_cast<Op>(OpCode::BRANCH) +
			   (cond << BRANCH_COND_BIT) +
			   (reg << BRANCH_REG_BIT) +
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

    void FENCE(Op &op) { op = static_cast<Op>(OpCode::FENCE); }

    void FUN(Op &op, Reg reg, PC end) {
      op = static_cast<Op>(static_cast<Op>(OpCode::FUN) + (reg << OP_CODE_BITS) + (end << FUN_END_BIT));
    }
        
    void GOTO(Op &op, PC pc) { op = static_cast<Op>(static_cast<Op>(OpCode::GOTO) + (pc << OP_CODE_BITS)); }
    
    void LOAD_BOOL(Op &op, Reg reg, bool val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_BOOL) +
			   (reg << LOAD_REG_BIT) +
			   ((val ? 1 : 0) << LOAD_VAL_BIT));
    }
    
    void LOAD_FUN(Op &op, Reg reg, snabl::Fun *val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_FUN) + (reg << OP_CODE_BITS));
      *(&op+1) = reinterpret_cast<Op>(val);
    }

    void LOAD_INT(Op &op, Reg reg, snabl::types::Int::DataType val) {
    }

    void LOAD_INT1(Op &op, Reg reg, snabl::types::Int::DataType val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_INT1) + (reg << LOAD_REG_BIT) + (val << LOAD_VAL_BIT));
    }

    void LOAD_INT2(Op &op, Reg reg, snabl::types::Int::DataType val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_INT2) + (reg << LOAD_REG_BIT));
      *(&op+1) = static_cast<Op>(val);
    }

    void LOAD_MACRO(Op &op, Reg reg, snabl::Macro *val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_MACRO) + (reg << LOAD_REG_BIT));
      *(&op+1) = reinterpret_cast<Op>(val);
    }

    void LOAD_TYPE(Op &op, Reg reg, snabl::Type val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_TYPE) +
			   (reg << LOAD_REG_BIT) +
			   (val.imp->id << LOAD_VAL_BIT));
    }

    void MOVE(Op &op, Reg dst, Reg src) {
      op = static_cast<Op>(static_cast<Op>(OpCode::MOVE) + (dst << MOVE_DST_BIT) + (src << MOVE_SRC_BIT));
    }

    void NOP(Op &op) { op = static_cast<Op>(OpCode::NOP); }
    void REC(Op &op) { op = static_cast<Op>(OpCode::REC); }
    void RET(Op &op) { op = static_cast<Op>(OpCode::RET); }

    void STATE(Op &op, int reg_count) {
      op = static_cast<Op>(static_cast<Op>(OpCode::STATE) + (reg_count << STATE_REG_COUNT_BIT));
    }

    void Z(Op &op, Reg dst, Reg src) {
      op = static_cast<Op>(static_cast<Op>(OpCode::Z) + (dst << Z_DST_BIT) + (src << Z_SRC_BIT));
    }

    void STOP(Op &op) { op = static_cast<Op>(OpCode::STOP); }
  }
}
