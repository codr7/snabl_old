#ifndef SNABL_OP_HPP
#define SNABL_OP_HPP

#include <cstdint>
#include "snabl/state.hpp"
#include "snabl/types/int.hpp"

#define OP_BITS 64
#define OP_CODE_BITS 6
#define OP_PC_BITS 10
#define OP_TYPE_ID_BITS 10

#define BENCH_REG_BIT OP_CODE_BITS
#define BENCH_END_BIT (BENCH_REG_BIT + OP_REG_BITS)

#define BRANCH_COND_BIT OP_CODE_BITS
#define BRANCH_REG_BIT (BRANCH_COND_BIT + OP_REG_BITS)
#define BRANCH_ELSE_BIT (BRANCH_REG_BIT + OP_REG_BITS)

#define CALL_TARGET_BIT OP_CODE_BITS
#define CALL_REG_BIT (CALL_TARGET_BIT + OP_REG_BITS)

#define CALLI1_REG_BIT OP_CODE_BITS
#define CALLI1_TARGET_BIT (CALLI1_REG_BIT + OP_REG_BITS)
#define CALLI1_TARGET_BITS (OP_BITS - CALLI1_TARGET_BIT - 1)
#define CALLI1_TARGET_MAX ((static_cast<Op>(1) << CALLI1_TARGET_BITS) - 1)

#define COPY_DST_BIT OP_CODE_BITS
#define COPY_SRC_BIT (COPY_DST_BIT + OP_REG_BITS)

#define DEC_DST_BIT OP_CODE_BITS
#define DEC_SRC_BIT (DEC_DST_BIT + OP_REG_BITS)
#define DEC_DELTA_BIT (DEC_SRC_BIT + OP_REG_BITS)
#define DEC_DELTA_BITS (OP_BITS - DEC_DELTA_BIT - 1)

#define EQ_DST_BIT OP_CODE_BITS
#define EQ_LEFT_BIT (EQ_DST_BIT + OP_REG_BITS)
#define EQ_RIGHT_BIT (EQ_LEFT_BIT + OP_REG_BITS)

#define FUN_REG_BIT OP_CODE_BITS
#define FUN_END_BIT (FUN_REG_BIT + OP_REG_BITS)

#define GOTO_PC_BIT OP_CODE_BITS

#define LOAD_INT1_VAL_BITS (OP_BITS - LOAD_VAL_BIT - 1)
#define LOAD_INT1_VAL_MIN (-(static_cast<types::Int::DataType>(1) << (LOAD_INT1_VAL_BITS - 1)))
#define LOAD_INT1_VAL_MAX ((static_cast<types::Int::DataType>(1) << (LOAD_INT1_VAL_BITS - 1)) - 1)

#define LOAD_REG_BIT OP_CODE_BITS
#define LOAD_VAL_BIT (LOAD_REG_BIT + OP_REG_BITS)

#define MOVE_DST_BIT OP_CODE_BITS
#define MOVE_SRC_BIT (MOVE_DST_BIT + OP_REG_BITS)

#define ONE_DST_BIT OP_CODE_BITS
#define ONE_SRC_BIT (ONE_DST_BIT + OP_REG_BITS)

#define STATE_BEG_REG_COUNT_BIT OP_CODE_BITS
#define STATE_END_REG_BIT OP_CODE_BITS

#define Z_DST_BIT OP_CODE_BITS
#define Z_SRC_BIT (Z_DST_BIT + OP_REG_BITS)

namespace snabl {
  using namespace std;

  struct Fun;
  struct Macro;
  
  using Op = uint64_t;

  enum class OpCode {
    BENCH, BRANCH,
    CALL, CALLI1, COPY,
    DEC, EQ,
    FENCE, FUN,
    GOTO,
    LOAD_BOOL, LOAD_FUN, LOAD_INT1, LOAD_INT2, LOAD_MACRO, LOAD_TYPE,
    MOVE, NOP, ONE, REC, RET, STATE_BEG, STATE_END, Z,
    /* STOP */
    STOP
  };
    
  inline OpCode op_code(Op op) {
    return static_cast<OpCode>(op & ((1 << OP_CODE_BITS) - 1));
  }

  PC op_len(Op op);
  bool op_reads(Op op, Reg reg);
  bool op_writes(Op op, Reg reg);
  void op_dump(Op op, ostream &out, M &m);

  namespace ops {
    template <typename T, size_t pos, size_t width>
    T get(Op op) { return static_cast<T>((op >> pos) & ((static_cast<T>(1) << width) - 1)); }

    void BENCH(Op &op, Reg reg, PC end_pc);
    inline Reg bench_reg(Op op) { return get<Reg, BENCH_REG_BIT, OP_REG_BITS>(op); }
    inline PC bench_end(Op op) { return get<PC, BENCH_END_BIT, OP_PC_BITS>(op); }

    void BRANCH(Op &op, Reg cond, Reg reg, PC else_pc);
    inline Reg branch_cond(Op op) { return get<Reg, BRANCH_COND_BIT, OP_REG_BITS>(op); }
    inline Reg branch_reg(Op op) { return get<Reg, BRANCH_REG_BIT, OP_REG_BITS>(op); }
    inline PC branch_else(Op op) { return get<PC, BRANCH_ELSE_BIT, OP_PC_BITS>(op); }
    
    void CALL(Op &op, Reg target, Reg reg);
    inline Reg call_target(Op op) { return get<Reg, CALL_TARGET_BIT, OP_REG_BITS>(op); }
    inline Reg call_reg(Op op) { return get<Reg, CALL_REG_BIT, OP_REG_BITS>(op); }

    void CALLI1(Op &op, Reg reg, Fun *target);
    inline Reg calli1_reg(Op op) { return get<Reg, CALLI1_REG_BIT, OP_REG_BITS>(op); }

    inline Fun *calli1_target(Op op) {
      return reinterpret_cast<Fun *>(get<Op, CALLI1_TARGET_BIT, CALLI1_TARGET_BITS>(op));
    }

    void COPY(Op &op, Reg dst, Reg src);
    inline Reg copy_dst(Op op) { return get<Reg, COPY_DST_BIT, OP_REG_BITS>(op); }
    inline Reg copy_src(Op op) { return get<Reg, COPY_SRC_BIT, OP_REG_BITS>(op); }

    void DEC(Op &op, Reg dst, Reg src, types::Int::DataType delta);
    inline Reg dec_dst(Op op) { return get<Reg, DEC_DST_BIT, OP_REG_BITS>(op); }
    inline Reg dec_src(Op op) { return get<Reg, DEC_SRC_BIT, OP_REG_BITS>(op); }
    inline types::Int::DataType dec_delta(Op op) { return get<types::Int::DataType, DEC_DELTA_BIT, DEC_DELTA_BITS>(op); }

    void EQ(Op &op, Reg dst, Reg left, Reg right);
    inline Reg eq_dst(Op op) { return get<Reg, EQ_DST_BIT, OP_REG_BITS>(op); }
    inline Reg eq_left(Op op) { return get<Reg, EQ_LEFT_BIT, OP_REG_BITS>(op); }
    inline Reg eq_right(Op op) { return get<Reg, EQ_RIGHT_BIT, OP_REG_BITS>(op); }

    void FENCE(Op &op);

    void FUN(Op &op, Reg reg, PC end);
    inline Reg fun_reg(Op op) { return get<Reg, FUN_REG_BIT, OP_REG_BITS>(op); }
    inline PC fun_end(Op op) { return get<PC, FUN_END_BIT, OP_PC_BITS>(op); }

    void GOTO(Op &op, PC pc);
    inline PC goto_pc(Op op) { return get<PC, GOTO_PC_BIT, OP_PC_BITS>(op); }

    void LOAD_BOOL(Op &op, Reg reg, bool val);
    inline bool load_bool_val(Op op) { return get<int, LOAD_VAL_BIT, 1>(op); }

    void LOAD_FUN(Op &op, Reg reg, snabl::Fun *val);

    void LOAD_INT1(Op &op, Reg reg, snabl::types::Int::DataType val);

    inline types::Int::DataType load_int1_val(Op op) {
      return get<types::Int::DataType, LOAD_VAL_BIT, LOAD_INT1_VAL_BITS>(op);
    }

    void LOAD_INT2(Op &op, Reg reg, snabl::types::Int::DataType val);

    void LOAD_MACRO(Op &op, Reg reg, snabl::Macro *val);

    void LOAD_TYPE(Op &op, Reg reg, Type val);
    inline snabl::Type::Id load_type_id(Op op) { return get<snabl::Type::Id, LOAD_VAL_BIT, OP_TYPE_ID_BITS>(op); }

    inline Reg load_reg(Op op) { return static_cast<Reg>((op >> OP_CODE_BITS) & ((1 << OP_REG_BITS) - 1)); }

    void MOVE(Op &op, Reg dst, Reg src);
    inline Reg move_dst(Op op) { return get<Reg, MOVE_DST_BIT, OP_REG_BITS>(op); }
    inline Reg move_src(Op op) { return get<Reg, MOVE_SRC_BIT, OP_REG_BITS>(op); }
    
    void NOP(Op &op);
    void REC(Op &op);

    void ONE(Op &op, Reg dst, Reg src);
    inline Reg one_dst(Op op) { return get<Reg, ONE_DST_BIT, OP_REG_BITS>(op); }
    inline Reg one_src(Op op) { return get<Reg, ONE_SRC_BIT, OP_REG_BITS>(op); }
    
    void RET(Op &op);
    
    void STATE_BEG(Op &op, int reg_count);
    inline int state_beg_reg_count(Op op) { return get<int, STATE_BEG_REG_COUNT_BIT, OP_REG_BITS>(op); }

    void STATE_END(Op &op, Reg reg);
    inline Reg state_end_reg(Op op) { return get<Reg, STATE_END_REG_BIT, OP_REG_BITS>(op); }

    void Z(Op &op, Reg dst, Reg src);
    inline Reg z_dst(Op op) { return get<Reg, Z_DST_BIT, OP_REG_BITS>(op); }
    inline Reg z_src(Op op) { return get<Reg, Z_SRC_BIT, OP_REG_BITS>(op); }

    /* STOP */
    
    void STOP(Op &op);
  }
}

#endif
