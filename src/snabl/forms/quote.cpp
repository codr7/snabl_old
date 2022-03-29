#include <iostream>

#include "snabl/m.hpp"
#include "snabl/forms/quote.hpp"

namespace snabl::forms {
  Quote::Quote(Pos pos, Form form): Form(make_shared<const Imp>(pos, form)) {}

  Quote::Imp::Imp(Pos pos, Form form): Form::Imp(pos), form(form) {}

  void Quote::Imp::dump(ostream &out) const { out << '\'' << form; }
  
  optional<Error> Quote::Imp::emit(Reg reg, M &m) const { return form.quote(reg, m); }
}
