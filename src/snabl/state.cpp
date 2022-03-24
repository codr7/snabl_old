#include "snabl/state.hpp"

namespace snabl {  
  State::State(State *outer, int reg_count): outer(outer), ref_count(1) {
    if (outer) {
      copy(outer->regs.begin(), outer->regs.begin() + reg_count, regs.begin());
      outer->ref_count++;
    }
  }
}
