#ifndef SNABL_SCOPE_HPP
#define SNABL_SCOPE_HPP

#include <array>

#include "snabl/val.hpp"

namespace snabl {
  using namespace std;

  struct Scope {
    Scope *outer;
    int ref_count;
    int reg_count;

    Scope(Scope *outer);
  };
}

#endif
