#ifndef SNABL_OP_HPP
#define SNABL_OP_HPP

#include <cstdint>
#include "snabl/state.hpp"
#include "snabl/types/int.hpp"

#define OP_BITS 64
#define OP_CODE_BITS 6
#define OP_PC_BITS 10
#define OP_REG_BITS 8
#define OP_TYPE_ID_BITS 10

#define BRANCH_COND_BIT OP_CODE_BITS
#define BRANCH_REG_BIT (BRANCH_COND_BIT + OP_REG_BITS)
#define BRANCH_ELSE_BIT (BRANCH_REG_BIT + OP_REG_BITS)

#define CALL_TARGET_BIT OP_CODE_BITS
#define CALL_REG_BIT (CALL_TARGET_BIT + OP_REG_BITS)

#define COPY_SRC_BIT (OP_CODE_BITS + OP_REG_BITS)

#define FUN_REG_BIT OP_CODE_BITS
#define FUN_END_BIT (FUN_REG_BIT + OP_REG_BITS)

#define LOAD_TYPE_ID_BIT (OP_CODE_BITS + OP_REG_BITS)
#define LOAD_VAL_BIT (OP_CODE_BITS + OP_REG_BITS)

namespace snabl {
  using namespace std;

  struct Fun;
  struct Macro;
  
  using Op = uint64_t;

  enum class OpCode {
    BRANCH,
    CALL, COPY,
    FUN, GOTO,
    LOAD_BOOL, LOAD_FUN, LOAD_INT, LOAD_MACRO, LOAD_TYPE,
    NOP, RET, STATE,
    /* STOP */
    STOP
  };
    
  inline OpCode op_code(Op op) {
    return static_cast<OpCode>(op & ((1 << OP_CODE_BITS) - 1));
  }

  PC op_len(Op op);

  void op_dump(Op op, ostream &out);

  namespace ops {
    template <typename T, size_t pos, size_t width>
    T get(Op op) { return static_cast<T>((op >> pos) & ((1 << width) - 1)); }
    
    void BRANCH(Op &op, Reg cond, Reg reg, PC else_pc);
    Reg branch_cond(Op op);
    Reg branch_reg(Op op);
    PC branch_else(Op op);    
    
    void CALL(Op &op, Reg target, Reg reg);
    Reg call_target(Op op);
    Reg call_reg(Op op);    

    void COPY(Op &op, Reg dst, Reg src);
    Reg copy_dst(Op op);
    Reg copy_src(Op op);    

    void FUN(Op &op, Reg reg, PC end);
    Reg fun_reg(Op op);
    PC fun_end(Op op);

    void GOTO(Op &op, PC pc);
    PC goto_pc(Op op);

    void LOAD_BOOL(Op &op, Reg reg, bool val);
    bool load_bool_val(Op op);
    void LOAD_FUN(Op &op, Reg reg, snabl::Fun *val);
    void LOAD_INT(Op &op, Reg reg, snabl::types::Int::DataType val);
    void LOAD_MACRO(Op &op, Reg reg, snabl::Macro *val);
    void LOAD_TYPE(Op &op, Reg reg, Type val);
    snabl::Type::Id load_type_id(Op op);
    Reg load_reg(Op op);

    void NOP(Op &op);
    void RET(Op &op);
    void STATE(Op &op);
    
    /* STOP */
    
    void STOP(Op &op);
  }
}

#endif
