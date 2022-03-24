#ifndef SNABL_STATE_HPP
#define SNABL_STATE_HPP

#include <array>

#include "snabl/val.hpp"

#define OP_REG_BITS 6

namespace snabl {
  using namespace std;
    
  struct State {
    static const int REG_COUNT = 1 << OP_REG_BITS;
    
    State *outer;
    int ref_count;
    array<optional<Val>, REG_COUNT> regs;

    State(State *outer, int reg_count);
  };
}

#endif
