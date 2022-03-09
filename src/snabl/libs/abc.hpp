#ifndef SNABL_LIBS_ABC_HPP
#define SNABL_LIBS_ABC_HPP

#include "snabl/lib.hpp"
#include "snabl/types/fun.hpp"
#include "snabl/types/int.hpp"
#include "snabl/types/meta.hpp"
#include "snabl/types/reg.hpp"

namespace snabl::libs {
  struct Abc: Lib {
    types::Fun fun_type;
    types::Int int_type;
    types::Meta meta_type;
    types::Reg reg_type;
    
    Abc(M &m);
  };
}

#endif
