#include "snabl/fun.hpp"

namespace snabl {
  Fun::Fun(Sym name, const vector<Arg> &args, Type ret_type, Body body):
    name(name), args(args), ret_type(ret_type), body(body) {
  }

  pair<PC, optional<Error>> Fun::call(PC ret_pc, M &m) {
    return body(*this, ret_pc, m);
  }
}
