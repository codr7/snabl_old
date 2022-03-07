#include "snabl/fun.hpp"
#include "snabl/scope.hpp"

namespace snabl {  
  Scope::Scope(Scope *outer): outer(outer), ref_count(1), reg_count(Fun::ARG_COUNT+1) {
    if (outer) {
      reg_count = outer->reg_count;
      outer->ref_count++;
    }
  }
}
