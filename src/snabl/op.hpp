#ifndef SNABL_OP_HPP
#define SNABL_OP_HPP

#include <cstdint>
#include "snabl/env.hpp"
#include "snabl/fun.hpp"

namespace snabl {
  using namespace std;

  using Op = uint64_t;

  #define OP_BITS 64
  #define OP_CODE_BITS 6
  #define OP_PC_BITS 10
  #define OP_REG_BITS 8
  #define OP_TYPE_ID_BITS 10

  enum class OpCode {
    GOTO, LOAD_FUN, LOAD_TYPE, NOP, RET,
    /* STOP */
    STOP
  };
    
  inline OpCode op_code(Op op) {
    return static_cast<OpCode>(op & ((1 << OP_CODE_BITS) - 1));
  }
  
  namespace ops {    
    #define LOAD_TYPE_ID_BIT (OP_CODE_BITS + OP_REG_BITS)

    void GOTO(Op &op, PC pc);
    PC goto_pc(Op op);

    void LOAD_FUN(Op &op, Reg reg, snabl::Fun *val);
    void LOAD_TYPE(Op &op, Reg reg, Type val);
    Reg load_reg(Op op);
    snabl::Type::Id load_type_id(Op op);

    void NOP(Op &op);
    void RET(Op &op);

    /* STOP */
    
    void STOP(Op &op);
  }
}

#endif
