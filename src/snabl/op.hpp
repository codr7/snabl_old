#ifndef SNABL_OP_HPP
#define SNABL_OP_HPP

#include <cstdint>
#include "snabl/env.hpp"

namespace snabl {
  using namespace std;

  using Op = uint64_t;

  #define OP_BITS 64
  #define OP_CODE_BITS 6
  #define OP_PC_BITS 10
  #define OP_REG_BITS 8
  #define OP_TYPE_ID_BITS 10

  enum class OpCode {
    GOTO, LOAD_FUN, NOP, RET,
    /* STOP */
    STOP
  };
    
  inline OpCode op_code(Op op) {
    return static_cast<OpCode>(op & ((1 << OP_CODE_BITS) - 1));
  }

  namespace ops {
    struct Fun;
    
    Op GOTO(PC pc);
    PC goto_pc(Op op);

    Reg load_reg(Op op);
    Op LOAD_FUN(Reg reg, Fun *val);

    Op NOP();
    Op RET();

    /* STOP */
    
    Op STOP();
  }
}

#endif
