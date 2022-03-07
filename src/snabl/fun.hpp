#ifndef SNABL_FUN_HPP
#define SNABL_FUN_HPP

#include <array>
#include <functional>
#include <optional>
#include <vector>

#include "snabl/error.hpp"
#include "snabl/op.hpp"
#include "snabl/sym.hpp"
#include "snabl/type.hpp"

namespace snabl {
  struct Form;
  struct M;

  struct Fun {
    struct Arg {
      Sym name;
      Type type;
    };
      
    using Body = function<pair<PC, optional<Error>> (Fun &self, PC ret_pc, M &m)>;
    static const int ARG_COUNT = 8;
    
    Fun(Sym name, const vector<Arg> &args, Type ret_type, Body body = nullptr);
    optional<Error> emit(Form body, M &m);
    pair<PC, optional<Error>> call(PC ret_pc, M &m);
    
    Sym name;
    array<Arg, ARG_COUNT> args;
    int arg_count;
    Type ret_type;
    Body body;
  };
}

#endif
