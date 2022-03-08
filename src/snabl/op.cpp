#include "snabl/op.hpp"

namespace snabl {
  namespace ops {
    void FUN(Op &op, Reg fun, PC end_pc) {
      op = static_cast<Op>(static_cast<Op>(OpCode::FUN) + (fun << OP_CODE_BITS) + (end_pc << FUN_END_PC_BIT));
    }
    
    PC fun_reg(Op op) {
      return static_cast<Reg>(op >> OP_CODE_BITS);
    }
    
    PC fun_end_pc(Op op) {
      return static_cast<PC>((op >> FUN_END_PC_BIT) & ((1 << OP_PC_BITS) - 1));
    }

    void GOTO(Op &op, PC pc) {
      op = static_cast<Op>(static_cast<Op>(OpCode::GOTO) + (pc << OP_CODE_BITS));
    }

    PC goto_pc(Op op) {
      return static_cast<PC>(op >> OP_CODE_BITS);
    }

    void LOAD_FUN(Op &op, Reg reg, snabl::Fun *val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_FUN) + (reg << OP_CODE_BITS));
      *(&op+1) = reinterpret_cast<Op>(val);
    }

    void LOAD_INT(Op &op, Reg reg, snabl::types::Int::DataType val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_FUN) + (reg << OP_CODE_BITS));
      *(&op+1) = static_cast<Op>(val);
    }

    void LOAD_TYPE(Op &op, Reg reg, snabl::Type val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::LOAD_TYPE) +
			   (reg << OP_CODE_BITS) +
			   (val.imp->id << LOAD_TYPE_ID_BIT));
    }

    Reg load_reg(Op op) {
      return static_cast<Reg>(op >> OP_CODE_BITS);
    }

    snabl::Type::Id load_type_id(Op op) {
      return static_cast<snabl::Type::Id>(op >> LOAD_TYPE_ID_BIT & ((1 << OP_TYPE_ID_BITS) - 1));
    }
    
    void NOP(Op &op) {
      op = static_cast<Op>(OpCode::NOP);
    }

    void RET(Op &op) {
      op = static_cast<Op>(OpCode::RET);      
    }
    
    void STOP(Op &op) {
      op = static_cast<Op>(OpCode::STOP);
    }
  }
}
