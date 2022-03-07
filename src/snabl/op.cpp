#include "snabl/op.hpp"

namespace snabl {
  namespace ops {
    Op GOTO(PC pc) {
      return static_cast<Op>(static_cast<Op>(OpCode::STOP) + (pc << OP_CODE_BITS));
    }

    PC goto_pc(Op op) {
      return static_cast<PC>(op >> OP_CODE_BITS);
    }

    Op NOP() {
      return static_cast<Op>(OpCode::NOP);
    }

    Op RET() {
      return static_cast<Op>(OpCode::RET);      
    }
    
    Op STOP() {
      return static_cast<Op>(OpCode::STOP);
    }
  }
}
