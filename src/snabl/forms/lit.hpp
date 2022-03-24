#ifndef SNABL_FORMS_LIT_HPP
#define SNABL_FORMS_LIT_HPP

#include "snabl/form.hpp"
#include "snabl/sym.hpp"

namespace snabl::forms {
  using namespace snabl;
  
  struct Lit: Form {
    struct Imp: Form::Imp {
      Val val;
      Imp(Pos pos, Type type, any data);    
      void dump(ostream& out) const override;
      optional<Error> emit(Reg reg, M &m) const override;
      bool is_atom() const override;
    };

    Lit(Pos pos, Type type, any data);
  };

}

#endif
