#ifndef SNABL_FUN_HPP
#define SNABL_FUN_HPP

#include <array>
#include <deque>
#include <functional>
#include <optional>
#include <vector>

#include "snabl/error.hpp"
#include "snabl/sym.hpp"
#include "snabl/type.hpp"

namespace snabl {
  struct State;
  struct Form;
  struct M;

  struct Fun {
    struct Arg {
      Sym name;
      Type type;

      Arg();
      Arg(Sym name, Type type);
    };

    using Result = pair<PC, optional<Error>>;
    using Body = function<Result (Fun &fun, Reg ret_reg, PC ret_pc, M &m)>;
    static const int ARG_COUNT = 8;
    
    Fun(Sym name, const vector<Arg> &args, Type ret_type, Body body);
    optional<Error> emit(deque<Form> body, Reg reg, M &m);
    pair<PC, optional<Error>> call(Reg reg, PC ret_pc, M &m);
    
    Sym name;
    array<Arg, ARG_COUNT> args;
    int arg_count;
    Type ret_type;
    Body body;
    int reg_count;
    Reg emit_reg;
    State *state;
  };

  ostream &operator <<(ostream &out, Fun *val);
}

#endif
