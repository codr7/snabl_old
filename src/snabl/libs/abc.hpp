#ifndef SNABL_LIBS_ABC_HPP
#define SNABL_LIBS_ABC_HPP

#include "snabl/lib.hpp"
#include "snabl/types/bool.hpp"
#include "snabl/types/fun.hpp"
#include "snabl/types/int.hpp"
#include "snabl/types/meta.hpp"
#include "snabl/types/nil.hpp"
#include "snabl/types/reg.hpp"

namespace snabl::libs {
  struct Abc: Lib {
    types::Bool bool_type;
    types::Fun fun_type;
    types::Int int_type;
    types::Meta meta_type;
    types::Nil nil_type;
    types::Reg reg_type;
    
    Abc(M &m);
  };
}

#endif
