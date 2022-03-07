#ifndef SNABL_TYPE_HPP
#define SNABL_TYPE_HPP

#include "snabl/sym.hpp"

namespace snabl {
  struct Type {
    using Id = uint64_t;

    struct Imp {
      Imp(Id id, Sym name);
      
      Id id;
      Sym name;
    };
    
    Type();
    Type(Id id, Sym name);
    shared_ptr<const Imp> imp;
  };

  bool operator==(Type lhs, Type rhs);
  ostream &operator <<(ostream &out, Type val);
}

#endif
