#include "snabl/fun.hpp"
#include "snabl/scope.hpp"

namespace snabl {  
  Scope::Scope(Scope *outer): outer(outer), ref_count(1), reg_count(Fun::ARG_COUNT+1) {
    if (outer) {
      reg_count = outer->reg_count;
      outer->ref_count++;
    }
  }

  optional<Val> Scope::find(Sym key) {
    auto found = bindings.find(key);
    if (found == bindings.end()) { return nullopt; }
    return found->second;
  }

  void Scope::bind(Sym key, Type type, any data) { bindings[key] = Val(type, data); }
}
