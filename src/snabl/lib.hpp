#ifndef SNABL_LIB_HPP
#define SNABL_LIB_HPP

#include <map>

#include "snabl/fun.hpp"
#include "snabl/macro.hpp"
#include "snabl/sym.hpp"
#include "snabl/val.hpp"

namespace snabl {
  struct M;
  
  struct Lib {
    M &m;
    Sym name;
    map<Sym, Val> bindings;
    
    Lib(M &m, Sym name);
    Type::Id type_id() const;
    void add_type(Type type);
    optional<Val> find(Sym name);
    void bind(Sym key, Type type, any data);
    Fun *bind_fun(Sym name, const vector<Fun::Arg> &args, Type ret_type, Fun::Body body);
    Macro *bind_macro(Sym name, int arg_count, Macro::Body body);
  };
}

#endif
