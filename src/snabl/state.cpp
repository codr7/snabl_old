#include "snabl/state.hpp"

namespace snabl {  
  State::State(State *outer): outer(outer), ref_count(1) {
    if (outer) {
      regs = outer->regs;
      outer->ref_count++;
    }
  }
}
