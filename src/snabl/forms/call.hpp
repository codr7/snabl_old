#ifndef SNABL_FORMS_CALL_HPP
#define SNABL_FORMS_CALL_HPP

#include <deque>

#include "snabl/form.hpp"
#include "snabl/sym.hpp"

namespace snabl::forms {
  using namespace snabl;
  
  struct Call: Form {
    struct Imp: Form::Imp {
      Form target;
      vector<Form> args;
      
      Imp(Pos pos, Form target, const deque<Form> &args);    
      void dump(ostream& out) const override;
      optional<Error> emit(Reg reg, M &m) const override;
    };

    Call(Pos pos, Form target, const deque<Form> &args);
  };

}

#endif
