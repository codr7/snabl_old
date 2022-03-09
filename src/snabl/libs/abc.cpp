#include "snabl/libs/abc.hpp"
#include "snabl/m.hpp"

namespace snabl::libs {
  Abc::Abc(M &m):
    Lib(m),
    fun_type(*this, m.sym("Fun")),
    int_type(*this, m.sym("Int")),
    meta_type(*this, m.sym("Meta")),
    reg_type(*this, m.sym("Reg")) {
    bind(m.sym("Fun"), meta_type, fun_type);
    bind(m.sym("Int"), meta_type, int_type);
    bind(m.sym("Meta"), meta_type, meta_type);
    bind(m.sym("Reg"), meta_type, reg_type);
  }
}
