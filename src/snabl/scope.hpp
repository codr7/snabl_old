#ifndef SNABL_SCOPE_HPP
#define SNABL_SCOPE_HPP

#include <array>
#include <map>

#include "snabl/val.hpp"

namespace snabl {
  using namespace std;

  struct Scope {
    Scope *outer;
    map<Sym, Val> bindings;
    int ref_count;
    int reg_count;

    Scope(Scope *outer);
    optional<Val> find(Sym key);
    void bind(Sym key, Type type, any data);
  };
}

#endif
