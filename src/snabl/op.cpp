#include "snabl/op.hpp"

namespace snabl {
  namespace ops {
    void GOTO(Op &op, PC pc) {
      op = static_cast<Op>(static_cast<Op>(OpCode::STOP) + (pc << OP_CODE_BITS));
    }

    PC goto_pc(Op op) {
      return static_cast<PC>(op >> OP_CODE_BITS);
    }

    void LOAD_FUN(Op &op, Reg reg, Fun *val) {
      op = static_cast<Op>(static_cast<Op>(OpCode::STOP) + (reg << OP_CODE_BITS));
      *(&op+1) = reinterpret_cast<Op>(val);
    }

    Reg load_reg(Op op) {
      return static_cast<Reg>(op >> OP_CODE_BITS);
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
