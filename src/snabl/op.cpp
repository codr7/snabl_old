#include "snabl/op.hpp"

namespace snabl {
   PC op_len(Op op, ostream &out) {
    switch (op_code(op)) {
    case OpCode::BRANCH:
    case OpCode::COPY:
    case OpCode::FUN:
    case OpCode::GOTO:
    case OpCode::LOAD_BOOL:
    case OpCode::LOAD_TYPE:
    case OpCode::NOP:
    case OpCode::RET:
    case OpCode::STATE:
    case OpCode::STOP:
      break;
    case OpCode::CALL:
    case OpCode::LOAD_FUN:
    case OpCode::LOAD_INT:
    case OpCode::LOAD_MACRO:
      return 2;
    }

    return 1;
  }

  void op_dump(Op op, ostream &out) {
    switch (op_code(op)) {
    case OpCode::BRANCH:
      out << "BRANCH " << ops::branch_cond(op) << ' ' << ops::branch_reg(op) << ' ' << ops::branch_else(op);
      break;
    case OpCode::CALL:
      out << "CALL " << ops::call_target(op) << ' ' << ops::call_reg(op);
      break;
    case OpCode::COPY:
      out << "COPY " << ops::copy_dst(op) << ' ' << ops::copy_src(op);
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
    case OpCode::LOAD_INT:
      out << "LOAD_INT " << ops::load_reg(op);
      break;
    case OpCode::LOAD_MACRO:
      out << "LOAD_MACRO " << ops::load_reg(op);
      break;
    case OpCode::LOAD_TYPE:
      out << "LOAD_TYPE " << ops::load_reg(op);
      break;
    case OpCode::NOP:
      out << "NOP";
      break;
    case OpCode::RET:
      out << "RET";
      break;
    case OpCode::STATE:
      out << "STATE";
      break;
    case OpCode::STOP:
      out << "STOP";
      break;
    }
  }

  namespace ops {    
    void BRANCH(Op &op, Reg cond, Reg reg, PC else_pc) {
      op = static_cast<Op>(static_cast<Op>(OpCode::BRANCH) +
			   (cond << BRANCH_COND_BIT) +
			   (reg << BRANCH_REG_BIT) +
			   (else_pc << BRANCH_ELSE_BIT));
    }

    Reg branch_cond(Op op) { return get<Reg, BRANCH_COND_BIT, OP_REG_BITS>(op); }

    Reg branch_reg(Op op) { return get<Reg, BRANCH_REG_BIT, OP_REG_BITS>(op); }

    PC branch_else(Op op) { return get<PC, BRANCH_ELSE_BIT, OP_PC_BITS>(op); }

    void CALL(Op &op, Reg target, Reg reg) {
      op = static_cast<Op>(static_cast<Op>(OpCode::CALL) + (target << CALL_TARGET_BIT) + (reg << CALL_REG_BIT));
    }

    Reg call_target(Op op) {
      return static_cast<Reg>((op >> CALL_TARGET_BIT) & ((1 << OP_REG_BITS) - 1));
    }
    
    Reg call_reg(Op op) {
      return static_cast<Reg>((op >> CALL_REG_BIT) & ((1 << OP_REG_BITS) - 1));
    }

    void COPY(Op &op, Reg dst, Reg src) {
      op = static_cast<Op>(static_cast<Op>(OpCode::COPY) + (dst << OP_CODE_BITS) + (src << COPY_SRC_BIT));
    }

    Reg copy_dst(Op op) {
      return static_cast<Reg>((op >> OP_CODE_BITS) & ((1 << OP_REG_BITS) - 1));
    }
    
    Reg copy_src(Op op) {
      return static_cast<Reg>((op >> COPY_SRC_BIT) & ((1 << OP_REG_BITS) - 1));
    }
    
    void FUN(Op &op, Reg reg, PC end) {
      op = static_cast<Op>(static_cast<Op>(OpCode::FUN) + (reg << OP_CODE_BITS) + (end << FUN_END_BIT));
    }
    
    Reg fun_reg(Op op) { return get<Reg, FUN_REG_BIT, OP_REG_BITS>(op); }
    
    PC fun_end(Op op) { return get<PC, FUN_END_BIT, OP_PC_BITS>(op); }
    
    void GOTO(Op &op, PC pc) {
      op = static_cast<Op>(static_cast<Op>(OpCode::GOTO) + (pc << OP_CODE_BITS));
    }

    PC goto_pc(Op op) { return static_cast<PC>(op >> OP_CODE_BITS); }

    void LOAD_BOOL(Op &op, Reg reg, bool val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_BOOL) +
			   (reg << OP_CODE_BITS) +
			   ((val ? 1 : 0) << LOAD_VAL_BIT));
    }

    bool load_bool_val(Op op) { return ((op >> LOAD_VAL_BIT) & 1) == 1; }
    
    void LOAD_FUN(Op &op, Reg reg, snabl::Fun *val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_FUN) + (reg << OP_CODE_BITS));
      *(&op+1) = reinterpret_cast<Op>(val);
    }

    void LOAD_INT(Op &op, Reg reg, snabl::types::Int::DataType val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_INT) + (reg << OP_CODE_BITS));
      *(&op+1) = static_cast<Op>(val);
    }

    void LOAD_MACRO(Op &op, Reg reg, snabl::Macro *val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_MACRO) + (reg << OP_CODE_BITS));
      *(&op+1) = reinterpret_cast<Op>(val);
    }

    void LOAD_TYPE(Op &op, Reg reg, snabl::Type val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_TYPE) +
			   (reg << OP_CODE_BITS) +
			   (val.imp->id << LOAD_TYPE_ID_BIT));
    }

    snabl::Type::Id load_type_id(Op op) {
      return static_cast<snabl::Type::Id>(op >> LOAD_TYPE_ID_BIT & ((1 << OP_TYPE_ID_BITS) - 1));
    }
    
    Reg load_reg(Op op) { return static_cast<Reg>((op >> OP_CODE_BITS) & ((1 << OP_REG_BITS) - 1)); }

    void NOP(Op &op) { op = static_cast<Op>(OpCode::NOP); }

    void RET(Op &op) { op = static_cast<Op>(OpCode::RET); }

    void STATE(Op &op) { op = static_cast<Op>(OpCode::STATE); }

    void STOP(Op &op) { op = static_cast<Op>(OpCode::STOP); }
  }
}
