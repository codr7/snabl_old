#ifndef SNABL_ENV_HPP
#define SNABL_ENV_HPP

#include <array>

#include "snabl/val.hpp"

namespace snabl {
  using namespace std;
    
  struct Env {
    static const int REG_COUNT = 256;
    
    Env *outer;
    int ref_count;
    array<Val, REG_COUNT> regs;

    Env(Env *outer);
  };
}

#endif
