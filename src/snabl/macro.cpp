#include "snabl/macro.hpp"
#include "snabl/m.hpp"

namespace snabl {
  Macro::Macro(Sym name, int arg_count, Body body):
    name(name), arg_count(arg_count), body(body) {}

  Macro::Result Macro::emit(deque<Form> args, Reg reg, Pos pos, M &m) {
    if (args.size() < arg_count) { return Error(pos, "Not enough arguments: ", name); }
    return body(*this, reg, pos, m);
  }

  ostream &operator <<(ostream &out, Macro *val) {
    out << "(Macro " << val->name << ')';
    return out;
  }
}
