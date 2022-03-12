#ifndef SNABL_FRAME_HPP
#define SNABL_FRAME_HPP

#include <array>

#include "snabl/op.hpp"
#include "snabl/val.hpp"

namespace snabl {
  using namespace std;

  struct Frame {
    Frame *outer;
    Reg ret_reg;
    PC ret_pc;
    int ref_count;

    Frame(Frame *outer, Reg ret_reg, PC ret_pc);
  };
}

#endif
