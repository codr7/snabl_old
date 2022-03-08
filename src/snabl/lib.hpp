#ifndef SNABL_LIB_HPP
#define SNABL_LIB_HPP

#include <map>

#include "snabl/sym.hpp"
#include "snabl/val.hpp"

namespace snabl {
  struct M;
  
  struct Lib {
    M &m;
    map<Sym, Val> bindings;
    
    Lib(M &m);
    Type::Id add_type(Type type);
    void bind(Sym key, Type type, any data);
  };
}

#endif
