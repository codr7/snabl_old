#include "snabl/libs/abc.hpp"
#include "snabl/m.hpp"

namespace snabl::libs {
  Abc::Abc(M &m): Lib(m), fun_type(m, m.sym("Fun")) {
  }
}
