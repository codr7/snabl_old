#include "snabl/glob.hpp"

namespace snabl {
  Glob::Glob(Sym id, Scope *scope, State *state): id(id) {}
  
  ostream &operator <<(ostream &out, Glob *val) {
    out << "(G " << val->id << ')';
    return out;
  }
}
