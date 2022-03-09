#ifndef SNABL_TYPE_HPP
#define SNABL_TYPE_HPP

#include <functional>
#include <optional>

#include "snabl/error.hpp"
#include "snabl/sym.hpp"
#include "snabl/types.hpp"

namespace snabl {
  struct Lib;
  struct M;
  struct Val;
  
  struct Type {
    using Id = uint64_t;

    struct Methods {
      function<void (Val val, ostream &out)> dump;
      function<optional<Error> (Val val, Reg reg, Pos pos, M &m)> emit;
      function<bool (Val val)> is_true;
    };
    
    struct Imp {
      Imp(Id id, Sym name);
      
      Id id;
      Sym name;
      Methods methods;
    };
    
    Type(shared_ptr<const Imp> imp = nullptr);
    Type(Lib &lib, Sym name);
    shared_ptr<const Imp> imp;
  };

  bool operator==(Type lhs, Type rhs);
  bool operator!=(Type lhs, Type rhs);
  ostream &operator <<(ostream &out, Type val);
}

#endif
