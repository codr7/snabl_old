#ifndef SNABL_SYM_HPP
#define SNABL_SYM_HPP

#include <string>

namespace snabl {
  using namespace std;
  
  struct Sym {
    using Id = int;

    Id id;
    string name;
    
    Sym(Id id, const string &name);
  };
}

#endif
