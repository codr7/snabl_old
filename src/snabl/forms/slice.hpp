#ifndef SNABL_FORMS_SLICE_HPP
#define SNABL_FORMS_SLICE_HPP

#include <deque>

#include "snabl/form.hpp"
#include "snabl/sym.hpp"

namespace snabl::forms {
  using namespace snabl;
  
  struct Slice: Form {
    struct Imp: Form::Imp {
      deque<Form> items;
      
      Imp(Pos pos, const deque<Form> &items);    
      void dump(ostream& out) const override;
      optional<Error> emit(Reg reg, M &m) const override;
      bool is_atom() const override;
    };

    Slice(Pos pos, const deque<Form> &items);
  };

}

#endif
