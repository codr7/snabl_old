#ifndef SNABL_LIBS_ABC_HPP
#define SNABL_LIBS_ABC_HPP

#include "snabl/lib.hpp"

namespace snabl::libs {
  struct Abc: Lib {
    Type bool_type, fun_type, int_type, macro_type, meta_type, nil_type, reg_type;
    
    Abc(M &m);
  };
}

#endif
