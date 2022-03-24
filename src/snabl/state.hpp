#ifndef SNABL_STATE_HPP
#define SNABL_STATE_HPP

#include <array>

#include "snabl/val.hpp"

namespace snabl {
  using namespace std;
    
  struct State {
    static const int REG_COUNT = 64;
    
    State *outer;
    int ref_count;
    array<optional<Val>, REG_COUNT> regs;

    State(State *outer);
  };
}

#endif
