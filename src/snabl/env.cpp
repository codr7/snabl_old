#include "snabl/env.hpp"

namespace snabl {  
  Env::Env(Env *outer): outer(outer), ref_count(1) {
    if (outer) {
      regs = outer->regs;
      outer->ref_count++;
    }
  }
}
