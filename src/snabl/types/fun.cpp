#include "snabl/m.hpp"
#include "snabl/types/fun.hpp"

namespace snabl::types {
  Fun::Imp::Imp(Id id, Sym name): Type::Imp(id, name) {
  }

  Fun::Fun(M &m, Sym name): Type() {
    imp = make_shared<const Imp>(m.add_type(*this), name);
  }
}
