#ifndef SNABL_GLOB_HPP
#define SNABL_GLOB_HPP

namespace snabl {
  struct Scope;
  
  struct Glob {
    Sym id;
    Scope *scope;
    State *state;
    Val val;

    Glob(Sym id);
  };

  ostream &operator <<(ostream &out, Glob *val);
}

#endif
