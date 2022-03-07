#include "snabl/op.hpp"

namespace snabl {
  namespace ops {
    Op GOTO(PC pc) {
      return static_cast<Op>(static_cast<Op>(OpCode::STOP) + (pc << OP_CODE_BITS));
    }
    
    Op NOP() {
      return static_cast<Op>(OpCode::NOP);
    }

    Op STOP() {
      return static_cast<Op>(OpCode::STOP);
    }
  }
}
