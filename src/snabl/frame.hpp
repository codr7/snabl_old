#ifndef SNABL_FRAME_HPP
#define SNABL_FRAME_HPP

#include <array>

#include "snabl/op.hpp"
#include "snabl/val.hpp"

namespace snabl {
  using namespace std;

  struct Frame {
    Frame *outer;
    Fun *target;
    Reg ret_reg;
    PC ret_pc;
    int ref_count;

    Frame(Frame *outer, Fun *target, Reg ret_reg, PC ret_pc):
      outer(outer), target(target), ret_reg(ret_reg), ret_pc(ret_pc), ref_count(1) {
      if (outer) { outer->ref_count++; }
    }
  };
}

#endif
