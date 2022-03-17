#ifndef SNABL_MACRO_HPP
#define SNABL_MACRO_HPP

#include <array>
#include <deque>
#include <functional>
#include <optional>

#include "snabl/error.hpp"
#include "snabl/form.hpp"
#include "snabl/sym.hpp"
#include "snabl/type.hpp"

namespace snabl {
  struct State;
  struct Form;
  struct M;

  struct Macro {
    using Result = optional<Error>;
    using Body = function<Result (Macro &macro, Pos pos, M &m)>;
    
    Macro(Sym name, int arg_count, Body body);
    optional<Error> emit(deque<Form> args, Pos pos, M &m);
    
    Sym name;
    int arg_count;
    Body body;
  };

  ostream &operator <<(ostream &out, Macro *val);
}

#endif
