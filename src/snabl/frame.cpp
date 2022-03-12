#include "snabl/fun.hpp"
#include "snabl/frame.hpp"

namespace snabl {  
  Frame::Frame(Frame *outer, Reg ret_reg, PC ret_pc): outer(outer), ret_reg(ret_reg), ret_pc(ret_pc), ref_count(1) {
    if (outer) { outer->ref_count++; }
  }
}
