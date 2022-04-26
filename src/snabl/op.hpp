#ifndef SNABL_OP_HPP
#define SNABL_OP_HPP

#include <cstdint>
#include "snabl/state.hpp"
#include "snabl/types/int.hpp"

#define OP_BITS 64
#define OP_CODE_BITS 6
#define OP_PC_BITS 10
#define OP_TYPE_ID_BITS 10

#define BENCH_REPS_BIT OP_CODE_BITS
#define BENCH_RES_BIT (BENCH_REPS_BIT + OP_REG_BITS)
#define BENCH_END_BIT (BENCH_RES_BIT + OP_REG_BITS)

#define BRANCH_COND_BIT OP_CODE_BITS
#define BRANCH_REG_BIT (BRANCH_COND_BIT + OP_REG_BITS)
#define BRANCH_IF_BIT (BRANCH_REG_BIT + OP_REG_BITS)
#define BRANCH_ELSE_BIT (BRANCH_IF_BIT + OP_PC_BITS)

#define CALL_TARGET_BIT OP_CODE_BITS
#define CALL_REG_BIT (CALL_TARGET_BIT + OP_REG_BITS)

#define CALLI1_REG_BIT OP_CODE_BITS
#define CALLI1_TARGET_BIT (CALLI1_REG_BIT + OP_REG_BITS)
#define CALLI1_TARGET_BITS (OP_BITS - CALLI1_TARGET_BIT - 1)
#define CALLI1_TARGET_MAX ((static_cast<Op>(1) << CALLI1_TARGET_BITS) - 1)

#define COPY_DST_BIT OP_CODE_BITS
#define COPY_SRC_BIT (COPY_DST_BIT + OP_REG_BITS)

#define COPYS_LEN_BITS OP_REG_BITS
#define COPYS_LEN_BIT (COPY_SRC_BIT + OP_REG_BITS)

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

#define LOAD_DST_BIT OP_CODE_BITS
#define LOAD_VAL_BIT (LOAD_DST_BIT + OP_REG_BITS)

#define MOVE_DST_BIT OP_CODE_BITS
#define MOVE_SRC_BIT (MOVE_DST_BIT + OP_REG_BITS)

#define MOVES_LEN_BITS OP_REG_BITS
#define MOVES_LEN_BIT (MOVE_SRC_BIT + OP_REG_BITS)

#define ONE_DST_BIT OP_CODE_BITS
#define ONE_SRC_BIT (ONE_DST_BIT + OP_REG_BITS)

#define RET_REG_BIT OP_CODE_BITS

#define STATE_BEG_NEXT_PC_BIT OP_CODE_BITS
#define STATE_BEG_COUNT_BIT (STATE_BEG_NEXT_PC_BIT+OP_PC_BITS)
#define STATE_BEG_COUNT_BITS 4

#define STATE_END_REG_BIT OP_CODE_BITS

#define TEST_EXPECTED_BIT OP_CODE_BITS
#define TEST_ACTUAL_BIT (TEST_EXPECTED_BIT + OP_REG_BITS)
#define TEST_RESULT_BIT (TEST_ACTUAL_BIT + OP_REG_BITS)

#define Z_DST_BIT OP_CODE_BITS
#define Z_SRC_BIT (Z_DST_BIT + OP_REG_BITS)

namespace snabl {
  using namespace std;

  struct Fun;
  struct Macro;
  
  using Op = uint64_t;

  enum class OpCode {
    BENCH, BRANCH,
    CALL, CALLI1, COPY, COPYS,
    DEC, EQ, FUN, GOTO,
    LOAD_BOOL, LOAD_FUN, LOAD_INT1, LOAD_INT2, LOAD_MACRO, LOAD_SYM, LOAD_TYPE,
    MOVE, MOVES,
    NOP, ONE,
    REC, RET,
    STATE_BEG, STATE_END,
    TEST, TRACE,
    Z,
    STOP
  };
    
  inline OpCode op_code(Op op) {
    return static_cast<OpCode>(op & ((1 << OP_CODE_BITS) - 1));
  }

  PC op_len(Op op);
  bool op_reads(Op op, Reg reg);
  bool op_writes(Op op, Reg reg);
  void op_trace(PC pc, ostream &out, M &m);

  namespace ops {
    template <typename T, size_t pos, size_t width>
    T get(Op op) { return static_cast<T>((op >> pos) & ((static_cast<T>(1) << width) - 1)); }

    void BENCH(Op &op, Reg reps, Reg res, PC end_pc);
    inline Reg bench_reps(Op op) { return get<Reg, BENCH_REPS_BIT, OP_REG_BITS>(op); }
    inline Reg bench_res(Op op) { return get<Reg, BENCH_RES_BIT, OP_REG_BITS>(op); }
    inline PC bench_end(Op op) { return get<PC, BENCH_END_BIT, OP_PC_BITS>(op); }

    void BRANCH(Op &op, Reg cond, Reg reg, PC if_pc, PC else_pc);
    inline Reg branch_cond(Op op) { return get<Reg, BRANCH_COND_BIT, OP_REG_BITS>(op); }
    inline Reg branch_reg(Op op) { return get<Reg, BRANCH_REG_BIT, OP_REG_BITS>(op); }
    inline PC branch_if_pc(Op op) { return get<PC, BRANCH_IF_BIT, OP_PC_BITS>(op); }
    inline PC branch_else_pc(Op op) { return get<PC, BRANCH_ELSE_BIT, OP_PC_BITS>(op); }
    
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

    void COPYS(Op &op, Reg dst, Reg src, int len);

    inline int copys_len(Op op) { return get<int, COPYS_LEN_BIT, COPYS_LEN_BITS>(op); }

    void DEC(Op &op, Reg dst, Reg src, types::Int::DataType delta);
    inline Reg dec_dst(Op op) { return get<Reg, DEC_DST_BIT, OP_REG_BITS>(op); }
    inline Reg dec_src(Op op) { return get<Reg, DEC_SRC_BIT, OP_REG_BITS>(op); }
    inline types::Int::DataType dec_delta(Op op) { return get<types::Int::DataType, DEC_DELTA_BIT, DEC_DELTA_BITS>(op); }

    void EQ(Op &op, Reg dst, Reg left, Reg right);
    inline Reg eq_dst(Op op) { return get<Reg, EQ_DST_BIT, OP_REG_BITS>(op); }
    inline Reg eq_left(Op op) { return get<Reg, EQ_LEFT_BIT, OP_REG_BITS>(op); }
    inline Reg eq_right(Op op) { return get<Reg, EQ_RIGHT_BIT, OP_REG_BITS>(op); }

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

    void LOAD_SYM(Op &op, Reg reg, Sym val);
    inline snabl::Sym::Id load_sym_id(Op op) { return get<snabl::Sym::Id, LOAD_VAL_BIT, OP_TYPE_ID_BITS>(op); }

    void LOAD_TYPE(Op &op, Reg reg, Type val);
    inline snabl::Type::Id load_type_id(Op op) { return get<snabl::Type::Id, LOAD_VAL_BIT, OP_TYPE_ID_BITS>(op); }

    inline Reg load_dst(Op op) { return get<Reg, LOAD_DST_BIT, OP_REG_BITS>(op); }

    void MOVE(Op &op, Reg dst, Reg src);
    inline Reg move_dst(Op op) { return get<Reg, MOVE_DST_BIT, OP_REG_BITS>(op); }
    inline Reg move_src(Op op) { return get<Reg, MOVE_SRC_BIT, OP_REG_BITS>(op); }

    void MOVES(Op &op, Reg dst, Reg src, int len);

    inline int moves_len(Op op) { return get<int, MOVES_LEN_BIT, MOVES_LEN_BITS>(op); }

    void NOP(Op &op);
    void REC(Op &op);

    void ONE(Op &op, Reg dst, Reg src);
    inline Reg one_dst(Op op) { return get<Reg, ONE_DST_BIT, OP_REG_BITS>(op); }
    inline Reg one_src(Op op) { return get<Reg, ONE_SRC_BIT, OP_REG_BITS>(op); }
    
    void RET(Op &op, Reg reg);
    inline Reg ret_reg(Op op) { return get<Reg, RET_REG_BIT, OP_REG_BITS>(op); }
    
    void STATE_BEG(Op &op, PC next, int count = 1);
    inline PC state_beg_next(Op op) { return get<PC, STATE_BEG_NEXT_PC_BIT, OP_PC_BITS>(op); }
    inline int state_beg_count(Op op) { return get<int, STATE_BEG_COUNT_BIT, STATE_BEG_COUNT_BITS>(op); }

    void STATE_END(Op &op, Reg reg);
    inline Reg state_end_reg(Op op) { return get<Reg, STATE_END_REG_BIT, OP_REG_BITS>(op); }

    void TEST(Op &op, Reg expected, Reg actual, Reg result);
    inline Reg test_expected(Op op) { return get<Reg, TEST_EXPECTED_BIT, OP_REG_BITS>(op); }
    inline Reg test_actual(Op op) { return get<Reg, TEST_ACTUAL_BIT, OP_REG_BITS>(op); }
    inline Reg test_result(Op op) { return get<Reg, TEST_RESULT_BIT, OP_REG_BITS>(op); }

    void TRACE(Op &op);

    void Z(Op &op, Reg dst, Reg src);
    inline Reg z_dst(Op op) { return get<Reg, Z_DST_BIT, OP_REG_BITS>(op); }
    inline Reg z_src(Op op) { return get<Reg, Z_SRC_BIT, OP_REG_BITS>(op); }

    void STOP(Op &op);
  }
}

#endif
