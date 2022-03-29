#ifndef SNABL_FORMS_QUOTE_HPP
#define SNABL_FORMS_QUOTE_HPP

#include "snabl/form.hpp"
#include "snabl/sym.hpp"

namespace snabl::forms {
  using namespace snabl;
  
  struct Quote: Form {
    struct Imp: Form::Imp {
      Form form;
      Imp(Pos pos, Form form);    
      void dump(ostream& out) const override;
      optional<Error> emit(Reg reg, M &m) const override;
    };

    Quote(Pos pos, Form form);
  };

}

#endif
