#ifndef SNABL_SYM_HPP
#define SNABL_SYM_HPP

#include <string>
#include <ostream>

namespace snabl {
  using namespace std;

  struct Sym {
    using Id = uint64_t;

    struct Imp {
      Imp(Id id, string name);
      
      Id id;
      string name;
    };
    
    Sym();
    Sym(Id id, string name);
    shared_ptr<const Imp> imp;
  };

  bool operator==(Sym lhs, Sym rhs);
  bool operator<(Sym lhs, Sym rhs);
  ostream &operator <<(ostream &out, Sym val);
}

#endif
