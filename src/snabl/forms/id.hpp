#ifndef SNABL_FORMS_ID_HPP
#define SNABL_FORMS_ID_HPP

#include "snabl/form.hpp"
#include "snabl/sym.hpp"

namespace snabl::forms {
  using namespace snabl;
  
  struct Id: Form::Imp {
    Id(Pos pos, Sym name);

    Sym name;
    
    void dump(ostream& out) const override;
    optional<Error> emit(Reg reg, M &m) const override;
  };

}

#endif
