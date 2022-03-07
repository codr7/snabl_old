#ifndef SNABL_OP_HPP
#define SNABL_OP_HPP

#include <cstdint>

namespace snabl {
  using namespace std;

  using PC = int64_t;
  using Op = uint64_t;

  #define OP_BITS 64
  #define OP_CODE_BITS 6
  #define OP_PC_BITS 10
  #define OP_REG_BITS 8
  #define OP_TYPE_ID_BITS 10

  enum class OpCode { STOP,
    GOTO, NOP
  };
    
  inline OpCode op_code(Op op) {
    return static_cast<OpCode>(op & ((1 << OP_CODE_BITS) - 1));
  }

  namespace ops {
    Op GOTO(PC pc);
    Op NOP();
    Op STOP();
  }
}

#endif
