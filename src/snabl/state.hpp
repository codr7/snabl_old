#ifndef SNABL_STATE_HPP
#define SNABL_STATE_HPP

#include <array>

#include "snabl/val.hpp"

namespace snabl {
  using namespace std;
    
  struct State {
    static const int REG_COUNT = 256;
    
    State *outer;
    int ref_count;
    array<Val, REG_COUNT> regs;

    State(State *outer);
  };
}

#endif
