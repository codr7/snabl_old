#ifndef SNABL_OP_HPP
#define SNABL_OP_HPP

#include <cstdint>
#include "snabl/state.hpp"
#include "snabl/fun.hpp"
#include "snabl/types/int.hpp"

#define OP_BITS 64
#define OP_CODE_BITS 6
#define OP_PC_BITS 10
#define OP_REG_BITS 8
#define OP_TYPE_ID_BITS 10

#define COPY_SRC_BIT (OP_CODE_BITS + OP_REG_BITS)
#define FUN_END_PC_BIT (OP_CODE_BITS + OP_REG_BITS)
#define LOAD_TYPE_ID_BIT (OP_CODE_BITS + OP_REG_BITS)

namespace snabl {
  using namespace std;

  using Op = uint64_t;

  enum class OpCode {
    COPY, FUN, GOTO,
    LOAD_FUN, LOAD_INT, LOAD_TYPE,
    NOP, RET,
    /* STOP */
    STOP
  };
    
  inline OpCode op_code(Op op) {
    return static_cast<OpCode>(op & ((1 << OP_CODE_BITS) - 1));
  }
  
  namespace ops {
    void COPY(Op &op, Reg dst, Reg src);
    Reg copy_dst(Op op);
    Reg copy_src(Op op);    

    void FUN(Op &op, Reg fun, PC end_pc);
    PC fun_reg(Op op);
    PC fun_end_pc(Op op);

    void GOTO(Op &op, PC pc);
    PC goto_pc(Op op);

    void LOAD_FUN(Op &op, Reg reg, snabl::Fun *val);
    void LOAD_INT(Op &op, Reg reg, snabl::types::Int::DataType val);
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
